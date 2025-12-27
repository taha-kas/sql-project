#include "schema.h"
#include <stdio.h>

void create_tables(sqlite3 *db) {

    char *err_msg = 0;
    int rc ;    

    const char *sql_sem = "Create table semester("
                          "semester_id text Primary key,"
                          "academic_year TEXT,"
                          "start_date DATE,"
                          "end_date DATE,"
                          "enrollment_deadline date);";  

    rc = sqlite3_exec(db,sql_sem,0,0,&err_msg);
    if(rc != SQLITE_OK) {
        printf("Erreur SQL : %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    printf("Table de semester créée avec succès !\n");

    const char *sql = "Create Table enrollment("
                      "enrollment_id integer Primary key AUTOINCREMENT,"  
                      "student_id integer,"
                      "major_id Text,"
                      "academic_year TEXT,"
                      "Foreign key (student_id) references student(id) ON DELETE CASCADE,"
                      "Foreign key (major_id) references major(major_id) ON DELETE CASCADE);";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if(rc != SQLITE_OK) {
        printf("Erreur SQL : %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    printf("Table de enrollment créée avec succès !\n");

    const char *sql1 = "Create Table Major_prerequisites("
                       "Major_id Text,"
                       "prerequisite_Major_id Text,"
                       "Primary key (Major_id,prerequisite_Major_id),"
                       "foreign key(Major_id) references Major(Major_id) ON DELETE CASCADE,"
                       "foreign key(prerequisite_Major_id) references Major(Major_id) ON DELETE CASCADE);";
                       
    rc = sqlite3_exec(db, sql1, 0, 0, &err_msg);
    if(rc != SQLITE_OK) {
        printf("Erreur SQL : %s\n", err_msg);
        sqlite3_free(err_msg);
        return ;
    }

    printf("Table de prerequisites crée avec succès !\n");

    const char *sql2 = "Create table admission_conditions("
                       "condition_id integer Primary key,"
                       "condition_type text,"
                       "ref_id text);";
    
    rc = sqlite3_exec(db, sql2, 0, 0, &err_msg);
    if(rc != SQLITE_OK) {
        printf("Erreur SQL : %s\n", err_msg);
        sqlite3_free(err_msg);
        return ;
    }
    printf("Table de admission_conditions crée avec succès !\n");


    const char *sql4 = "Create table student_concour("
                       "student_id integer ,"
                       "concours_id text,"
                       "admitted integer,"
                       "PRIMARY KEY (student_id, concours_id),"
                       "FOREIGN KEY (student_id) REFERENCES student(id) ON DELETE CASCADE);";

    rc = sqlite3_exec(db, sql4, 0, 0, &err_msg);
    if(rc != SQLITE_OK) {
        printf("Erreur SQL : %s\n", err_msg);
        sqlite3_free(err_msg);
        return ;
    }
    printf("Table de student_concour crée avec succès !\n");

    // Création de la table grades
    const char *sql5 =
        "CREATE TABLE IF NOT EXISTS grade ("
        "grade_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "student_id INTEGER NOT NULL,"
        "course_id text NOT NULL,"
        "semester TEXT ,"
        "academic_year TEXT NOT NULL,"
        "CC1 REAL NOT NULL,"
        "CC2 REAL NOT NULL,"
        "NOTE REAL NOT NULL,"
        "FOREIGN KEY (student_id) REFERENCES student(id) ON DELETE CASCADE,"
        "FOREIGN KEY (course_id) REFERENCES course(course_id) ON DELETE CASCADE"
        ");";
    
    rc = sqlite3_exec(db, sql5, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        printf("Erreur SQL (create table): %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    printf("Table grade créée avec succès.\n");

    return ;

}