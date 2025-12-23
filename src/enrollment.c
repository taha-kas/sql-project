#include <stdio.h>
#include <string.h>
#include "database.h"
#include "enrollment.h"
#include "condition.h"


//fct +
int StudentExistsInDB(sqlite3 *db, int student_id){

    int rc;
    sqlite3_stmt *stmt;

    const char *sql = "SELECT 1 FROM student WHERE id = ?;";
    
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Erreur prepare étudiant : %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, student_id);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return 1; // Student exists
    } else {
        sqlite3_finalize(stmt);
        return 0; // Student does not exist
    }
}



int vérifierPre(sqlite3 *db,Major_List *major_list, int student_id,char *major_id,char *semester){

    int rc;
    sqlite3_stmt *stmt;
    const char *course_id;
    float grade = 0;
    int compensé=0;
    float Overall_grade = 0;


    sqlite3_stmt *stmt1;
    const char *sql = "select prerequisite_Major_id From Major_prerequisites where Major_id = ?;";
    rc = sqlite3_prepare_v2(db,sql,-1,&stmt1,NULL);

    if (rc != SQLITE_OK) {
        printf("Erreur prepare Major_prerequisites: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt1,1,major_id,-1,SQLITE_TRANSIENT);
    rc = sqlite3_step(stmt1);

    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt1);
        printf("Major est accessible sans prerequis");
        return 1; // pas de prérequis → OK
    }

    const unsigned char *prerequisite_major_id = sqlite3_column_text(stmt1, 0);
    sqlite3_finalize(stmt1);

    Major *temp = major_list->head;
    while (temp!=NULL && strcmp(temp -> major_id, prerequisite_major_id) != 0)
    {
        temp = temp->next;
    }
    Course *element = temp ->course_list->head;
    while (element!=NULL){
        course_id = element->course_id;

        const unsigned char *sql_grade = "select NOTE from grade where student_id = ? and course_id = ? and semester_id = ?;";

        rc = sqlite3_prepare_v2(db,sql_grade,-1,&stmt,NULL);
        if (rc != SQLITE_OK) {
        printf("Erreur prepare grades : %s\n", sqlite3_errmsg(db));
        return 0;
        }
        sqlite3_bind_int(stmt,1,student_id);
        sqlite3_bind_text(stmt,2,course_id,-1,SQLITE_STATIC);
        sqlite3_bind_text(stmt,3,semester,-1,SQLITE_STATIC);
        
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_ROW) {
            sqlite3_finalize(stmt);
            printf("Module non validé à cause de l'abscence injustifié dans l'examen");
            return 0; // module non validé
        }
        grade = sqlite3_column_int(stmt, 0);
        Overall_grade += grade;

        if (grade < 7) {
            sqlite3_finalize(stmt);
            printf("major non validé à cause de note éliminatoire dans le module %s",course_id);
            return 0;
        }

        if (grade < 10)
            compensé++;
        
        sqlite3_finalize(stmt);
        element = element->next;

    }


    if(compensé <= 3 && Overall_grade/temp->course_list->num_of_courses >= 10){
        printf("Major validé");
        return 1;
    }else
    {
        printf("major non validé ");
        return 0;
    }
    
    }


int enrollStudent(sqlite3 *db,Major_List *major_list, int student_id, char *major_id, char *semester){

    int rc;
    int état;

    sqlite3_stmt *stmt;

    // --- Vérifier si l'étudiant est interne ou externe ---
    // 0 = interne et 1 = externe

    
    const char *sql = "SELECT 1 FROM student WHERE student_id = ?;";
    
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Erreur prepare étudiant : %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_int(stmt, 1, student_id);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        état = 1;
    }else if (rc == SQLITE_ROW)
    {
        état = 0;
    }
    

    sqlite3_finalize(stmt);

    // --- Vérifier si le cours existe ---
    const char *sql1 = "SELECT 1 FROM Major WHERE major_id = ?;";

    rc = sqlite3_prepare_v2(db, sql1, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Erreur prepare cours : %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_text(stmt, 1, major_id,-1,SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        printf("Cours n'existe pas\n");
        sqlite3_finalize(stmt);
        return 1;
    }

    sqlite3_finalize(stmt);
    
    //---- Verifier que étudiant n'est pas déjà inscris

    rc = sqlite3_prepare_v2(db,"select * from enrollment where student_id = ?;",-1,&stmt,NULL);

    if (rc != SQLITE_OK) {
        printf("Erreur prepare enrollment : %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_int(stmt, 1,student_id);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        printf("Etudiant déjà inscris \n");
        sqlite3_finalize(stmt);
        return 1;
    }

    sqlite3_finalize(stmt);

    //------- Verifier la capacité -------

    const char *sql2 = "select capacity,enrolled_students from Major where major_id = ?;";
    rc = sqlite3_prepare_v2(db,sql2,-1,&stmt,NULL);
    if (rc != SQLITE_OK) {
        printf("Erreur prepare major : %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_text(stmt,1,major_id,-1,SQLITE_TRANSIENT);
    rc = sqlite3_step(stmt);
    if(rc == SQLITE_ROW){
        int capacity = sqlite3_column_int(stmt,0);
        int enrolled_students = sqlite3_column_int(stmt,1);
        if (enrolled_students >= capacity)
        {
            printf("The major is full\n");
            sqlite3_finalize(stmt);
            return 1;
        }
        
    }else
    {
        printf("Major n'existe pas \n");
        sqlite3_finalize(stmt);
        return 1;
    }
    
    sqlite3_finalize(stmt);


    //------- Vérifier prérequis  --------

    if (état == 0) //interne
    {
        if(!vérifierPre(db,major_list,student_id,major_id,semester)){
            printf("Major prerequisite non validé");
            return 1;
        }
    }else if (état == 1)
    {
        const char *ref_id;
        int a=0;
        while (1)
        {
            printf("\n---------------------------------\n");
            printf("\nWhat's your previous insitituion\n");
            printf("\n1. Classe preparatoire");
            printf("\n2. Est, Facultés, Other");
            scanf("%d",&a);

            if (a!=1 && a!=2)
            {
               printf("Choice invalid, choose 1 or 2");
            }else
            {
                break;
            }
        }
    

        if (a==1)
        {
            const char *sql_cond = "SELECT ref_id FROM admission_conditions WHERE condition_type = ?;";
            rc = sqlite3_prepare_v2(db,sql_cond,-1,&stmt,NULL);
            if (rc != SQLITE_OK) {
                printf("Erreur prepare admission_conditions : %s\n", sqlite3_errmsg(db));
                return 1;
            } 
            sqlite3_bind_text(stmt,1,"CNC",-1,SQLITE_TRANSIENT);

            rc = sqlite3_step(stmt);
            if(rc == SQLITE_ROW){
                ref_id = sqlite3_column_text(stmt,0);
            }

            sqlite3_stmt *stmt_cnc;
            const char* sql_cnc = "select student_id,admitted from student_concour where student_id = ? and concours_id=?;";
            rc=sqlite3_prepare_v2(db,sql_cnc,-1,&stmt_cnc,NULL);
            if (rc != SQLITE_OK) {
                printf("Erreur prepare student_concours : %s\n", sqlite3_errmsg(db));
                sqlite3_finalize(stmt_cnc);
                return 1;
            }
        
            sqlite3_bind_int(stmt_cnc,1,student_id);
            sqlite3_bind_text(stmt_cnc,2,ref_id,-1,SQLITE_TRANSIENT);

            rc = sqlite3_step(stmt_cnc);
            if (rc == SQLITE_ROW) {
                int admitted = sqlite3_column_int(stmt_cnc,1);
            if (admitted == 0) //admitted == 0 -> has failed CNC
            sqlite3_finalize(stmt_cnc);
            return 1;
            }
        sqlite3_finalize(stmt_cnc);

        }else if (a==2)
        {
            const char *sql_cond = "SELECT ref_id FROM admission_conditions WHERE condition_type = ?;";
            rc = sqlite3_prepare_v2(db,sql_cond,-1,&stmt,NULL);
            if (rc != SQLITE_OK) {
                printf("Erreur prepare admission_conditions : %s\n", sqlite3_errmsg(db));
                return 1;
            } 
            sqlite3_bind_text(stmt,1,"Concour interne",-1,SQLITE_TRANSIENT);

            rc = sqlite3_step(stmt);
            if(rc == SQLITE_ROW){
                ref_id = sqlite3_column_text(stmt,0);
            }

            sqlite3_stmt *stmt_cnc;
            const char* sql_cnc = "select student_id,admitted from student_concour where student_id = ? and concours_id=?;";
            rc=sqlite3_prepare_v2(db,sql_cnc,-1,&stmt_cnc,NULL);
            if (rc != SQLITE_OK) {
                printf("Erreur prepare student_concours : %s\n", sqlite3_errmsg(db));
                return 1;
            }
        
            sqlite3_bind_int(stmt_cnc,1,student_id);
            sqlite3_bind_text(stmt_cnc,2,ref_id,-1,SQLITE_TRANSIENT);

            rc = sqlite3_step(stmt_cnc);
            if (rc == SQLITE_ROW) {
                int admitted = sqlite3_column_int(stmt_cnc,1);
            if (admitted == 0) //admitted == 0 -> has failed CONCOUR INTERNE
            return 1;
            }
        }
        
        
        
    }

    // ------- Vérification de semester if enrollment is late
    
    const char *sql_late = "SELECT DATE('now') > enrollment_deadline FROM semester WHERE semester_id = ?;";

    sqlite3_prepare_v2(db, sql_late, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1,semester,-1,SQLITE_TRANSIENT);
    sqlite3_step(stmt);

    int isLate = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    if (isLate==1)
    {
        printf("inscription tardive\n");
        return 1;
    }
    


    //------- Vérification si on a établit les 5 conditions --------
    
    
    rc = sqlite3_prepare_v2(db,"insert into enrollment (student_id, major_id, semester) Values (?,?,?);",-1,&stmt,NULL);
    if (rc != SQLITE_OK) {
    printf("Erreur prepare enrollment : %s\n", sqlite3_errmsg(db));
    return 1;
    }

    sqlite3_bind_int(stmt,1,student_id);
    sqlite3_bind_text(stmt,2,major_id,-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,3,semester,-1,SQLITE_TRANSIENT);
    sqlite3_step(stmt); 
    sqlite3_reset(stmt);

    sqlite3_finalize(stmt);
    
    // je dois modifier enrolled_students in major
    const char *sql_st = "Update Major Set enrolled_students = enrolled_students + 1 where major_id = ?;";

    rc = sqlite3_prepare_v2(db,sql_st,-1,&stmt,NULL);
    if (rc != SQLITE_OK) {
    printf("Erreur prepare Major : %s\n", sqlite3_errmsg(db));
    return 1;
    }

    sqlite3_bind_text(stmt, 1, major_id,-1,SQLITE_TRANSIENT);
    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        printf("Erreur execution UPDATE Major\n");
        sqlite3_finalize(stmt);
        return 1;
        }

    sqlite3_finalize(stmt);

    return 0;
    
}

int unenrollStduent(sqlite3 *db, int student_id, char *major_id){

    int rc;
    sqlite3_stmt *stmt;

    // ---- Vérifier si l'étudiant existe dans la table student ----
    rc = sqlite3_prepare_v2(db,"SELECT 1 FROM student WHERE student_id = ?;",-1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        printf("Erreur prepare student : %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_int(stmt, 1, student_id);

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_DONE) {
        // Aucun étudiant trouvé
        printf("Étudiant n'existe pas !\n");
        sqlite3_finalize(stmt);
        return 1;
    }

    sqlite3_finalize(stmt);  // IMPORTANT

    // ---- Supprimer l'inscription ----

    rc = sqlite3_prepare_v2(db,"DELETE FROM enrollment WHERE student_id = ? AND major_id = ?;",-1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        printf("Erreur prepare delete : %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_int(stmt, 1, student_id);
    sqlite3_bind_text(stmt, 2, major_id,-1,SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_DONE) {
        printf("Row deleted successfully!\n");
    } else {
        printf("Aucune inscription trouvée pour cet étudiant.\n");
    }

    sqlite3_finalize(stmt);
    return 0;

}


