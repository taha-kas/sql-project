#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include "grade.h"
#include "course_list.h"
#include "database.h"

int verifier_etud_existe(sqlite3 *db, int student_id, char *course_id, char *semester){
    
    int rc;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT 1 FROM grade WHERE student_id = ? and course_id = ? and semester_ = ?;";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Erreur prepare grade : %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, student_id);
    sqlite3_bind_text(stmt,2,course_id,-1,SQLITE_STATIC);
    sqlite3_bind_text(stmt,3,semester,-1,SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_ROW); 
    
}


int insert_into_grade_table(sqlite3 *db, int student_id, char *course_id, char *semester){
    int rc;
    sqlite3_stmt *stmt;
    // --- Vérifier si l'étudiant existe ---

    const unsigned char *sql = "SELECT 1 FROM student WHERE student_id = ?;";
    
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Erreur prepare étudiant : %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_int(stmt, 1, student_id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc == SQLITE_DONE) {
        printf("Étudiant n'existe pas\n");
        return 1;
    }

        // --- Vérifier si le cours existe ---
    const char *sql1 = "SELECT 1 FROM Course WHERE course_id = ?;";

    rc = sqlite3_prepare_v2(db, sql1, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Erreur prepare cours : %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_text(stmt, 1, course_id,-1,SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc == SQLITE_DONE) {
        printf("Course n'existe pas\n");
        sqlite3_finalize(stmt);
        return 1;
    }

    // verifier si elle est deja dans base de donnes
    
    if(verifier_etud_existe(db,student_id,course_id,semester)){
        return 1;
    }

    // ajouter si elle n'est pas dans base de données

    const char *sql_aj = "insert into grade(student_id,course_id,semester, CC1, CC2, NOTE) values (?,?,?,0,0,0);";
        sqlite3_prepare_v2(db,sql_aj,-1,&stmt,NULL);

        sqlite3_bind_int(stmt,1,student_id);
        sqlite3_bind_text(stmt,2,course_id,-1,SQLITE_STATIC);
        sqlite3_bind_text(stmt,3,semester,-1,SQLITE_STATIC);

        sqlite3_step(stmt); 

        sqlite3_finalize(stmt);
        printf("Etudiant ajouter dans base de données");
}



int addGrade(sqlite3 *db, int student_id, char* course_id, char *semester,float CC1, float CC2){

    int rc;
    sqlite3_stmt *stmt;

    float NOTE = CC1 * 0.3f + CC2 * 0.7f;
    insert_into_grade_table(db,student_id,course_id,semester);


    const char *sql = "UPDATE grade SET CC1 = ?, CC2 = ?, NOTE = ? WHERE student_id = ? AND course_id = ? AND semester = ?;";
    sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);

    sqlite3_bind_double(stmt,1,CC1);
    sqlite3_bind_double(stmt,2,CC2);
    sqlite3_bind_double(stmt,3,NOTE);
    sqlite3_bind_int(stmt,4,student_id);
    sqlite3_bind_text(stmt,5,course_id,-1,SQLITE_STATIC);
    sqlite3_bind_text(stmt,6,semester,-1,SQLITE_STATIC);

    sqlite3_step(stmt); 
    sqlite3_finalize(stmt);
    printf("\ngrade ajouter avec succès\n");
    return 1;
    
}

int updateGrade(sqlite3 *db, int student_id, char* course_id, char *semester, float new_CC1, float new_CC2) {
    sqlite3_stmt *stmt;
    int rc;

    float new_NOTE = new_CC1 * 0.3f + new_CC2 * 0.7f;

    // Vérifier si la ligne existe
    if (!verifier_etud_existe(db, student_id, course_id, semester)) {
        printf("Erreur : l'étudiant n'est pas inscrit à ce cours pour ce semestre.\n");
        return 1;
    }

    // Mettre à jour les notes
    const char *sql_update =
        "UPDATE grade "
        "SET CC1 = ?, CC2 = ?, NOTE = ? "
        "WHERE student_id01 = ? AND course_id01 = ? AND semester = ?;";

    rc = sqlite3_prepare_v2(db, sql_update, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Erreur prepare update : %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_double(stmt, 1, new_CC1);
    sqlite3_bind_double(stmt, 2, new_CC2);
    sqlite3_bind_double(stmt, 3, new_NOTE);
    sqlite3_bind_int(stmt, 4, student_id);
    sqlite3_bind_text(stmt, 5, course_id, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, semester, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        printf("Erreur lors de la mise à jour des notes : %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 1;
    }

    sqlite3_finalize(stmt);
    printf("Note modifiée avec succès !\n");
    return 0;
}


int affichage_grade(sqlite3 *db,Major_List *major_list, int student_id,char *major_id,char *semester){
    int rc;
    sqlite3_stmt *stmt2;
    const char *course_id;
    float grade;

    Major *temp = major_list->head;
    while (temp!=NULL && strcmp(temp->major_id,major_id)!=0)
    {
        temp = temp->next;
    }

    if (temp==NULL)
    {
        printf("Major n'existe pas ");
    }
    
    Course *element = temp ->course_list->head;
    initialisation_course();
    
    while (element!=NULL){
        course_id=element->course_id;
        const unsigned char *sql_grade = "select NOTE from grade where student_id = ? and course_id = ? and semester = ?;";

        rc = sqlite3_prepare_v2(db,sql_grade,-1,&stmt2,NULL);
        if (rc != SQLITE_OK) {
        printf("Erreur prepare grades : %s\n", sqlite3_errmsg(db));
        return 0;
        }

        sqlite3_bind_int(stmt2,1,student_id);
        sqlite3_bind_text(stmt2,2,course_id,-1,SQLITE_STATIC);
        sqlite3_bind_text(stmt2,3,semester,-1,SQLITE_STATIC);
        
        if (sqlite3_step(stmt2) == SQLITE_ROW) {
            grade = sqlite3_column_double(stmt2, 0);
            }else{
                grade = 0; // ou continuer
            }

        ajouter_tete((char*)course_id,grade);
        sqlite3_finalize(stmt2);
        element = element->next;
    }
    printf("| %6d | %7s | %10s\n",student_id,major_id,semester);
    affichage();
    liberer_course();
    return 1;
  
}