#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include "sqlite3.h"
#include "database.h"

/////////////////////////////////////STUDENT DATABASE FUNCTIONS/////////////////////////////////////
int db_connect(sqlite3 **db, char* db_name){
    if (sqlite3_open(db_name, db) != SQLITE_OK) {
        printf("Error opening DB: %s\n", sqlite3_errmsg(*db));
        return 0;
    }
    printf("Database opened successfully!\n"); 
    return 1;
}


int CreateStudentTable(sqlite3 *db){
char* sql = "CREATE TABLE IF NOT EXISTS student ("
      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
      "first_name TEXT NOT NULL, "
      "last_name TEXT NOT NULL, "
      "date_of_birth TEXT NOT NULL, "
      "status TEXT NOT NULL);";


int rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
if (rc != SQLITE_OK) {
    printf("Create table failed: %s\n", sqlite3_errmsg(db));
    return 0;
}

return 1;
 
}


int db_insert_student(sqlite3 *db, Student* s){
    sqlite3_stmt* stmt; 

    sqlite3_prepare_v2(
        db,
        "INSERT INTO student(first_name, last_name, date_of_birth, status)"
        "VALUES (?,?,?,?)",
        -1,
        &stmt,
        NULL
    );
    
    sqlite3_bind_text(stmt, 1, s -> first_name, -1, SQLITE_TRANSIENT); 
    sqlite3_bind_text(stmt, 2, s -> last_name, -1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, s -> date_of_birth, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, s -> status, -1, SQLITE_TRANSIENT); 
    
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Insert failed: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    } 
    sqlite3_finalize(stmt); 

    s -> id = (int)sqlite3_last_insert_rowid(db);
    
    return 1; 
}

void load_students(sqlite3 *db, Student_List* list){
   sqlite3_stmt* stmt; 
   
   if (sqlite3_prepare_v2(
        db,
        "SELECT * FROM student",
        -1,
        &stmt,
        NULL) != SQLITE_OK){
            printf("Error preparing SELECT statement: %s\n", sqlite3_errmsg(db));
            return;
   }

   while(sqlite3_step(stmt) == SQLITE_ROW){
        const char* fn = (const char*) sqlite3_column_text(stmt, 1);
        const char* ln = (const char*) sqlite3_column_text(stmt, 2);
        const char* dob = (const char*) sqlite3_column_text(stmt, 3);
        const char* status = (const char*) sqlite3_column_text(stmt, 4);
        Student* temp = malloc(sizeof(Student)); 
        temp -> first_name = strdup(fn);
        temp -> last_name = strdup(ln);
        temp -> date_of_birth = strdup(dob);
        temp -> status = strdup(status);
        temp -> next = NULL;

        temp->id = sqlite3_column_int(stmt, 0);
        Insert_student_list(list, temp);
   }

   sqlite3_finalize(stmt); 
}

void db_remove_student(sqlite3 *db, int id){
    sqlite3_stmt* stmt; 

    sqlite3_prepare_v2(
        db,
        "DELETE FROM student WHERE id = ?",
        -1,
        &stmt,
        NULL
    );
    
    sqlite3_bind_int(stmt, 1, id); 

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Delete failed: %s\n", sqlite3_errmsg(db));
    } 
    sqlite3_finalize(stmt); 
}

void db_update_student(sqlite3 *db, Student s){
    sqlite3_stmt* stmt; 

    sqlite3_prepare_v2(
        db,
        "UPDATE student SET first_name = ?, last_name = ?, date_of_birth = ?, status = ? WHERE id = ?",
        -1,
        &stmt,
        NULL
    );
    
    sqlite3_bind_text(stmt, 1, s.first_name, -1, SQLITE_TRANSIENT); 
    sqlite3_bind_text(stmt, 2, s.last_name, -1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, s.date_of_birth, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, s.status, -1, SQLITE_TRANSIENT); 
    sqlite3_bind_int(stmt, 5, s.id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Update failed: %s\n", sqlite3_errmsg(db));
    } 
    sqlite3_finalize(stmt); 
}

/////////////////////////////////////END OF STUDENT DATABASE FUNCTIONS/////////////////////////////////////


/////////////////////////////////////COURSE DATABASE FUNCTIONS////////////////////////////////////////////

void CreateCourseTable(sqlite3* db){
    char* sql = "CREATE TABLE IF NOT EXISTS course ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "course_name TEXT NOT NULL, "
                "course_code TEXT NOT NULL, "
                "credits INTEGER NOT NULL);";


    if(sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK){
        printf("Create course table failed: %s\n", sqlite3_errmsg(db));
    }
}

void db_insert_course(sqlite3* db, Course* c){
    sqlite3_stmt* stmt; 

    sqlite3_prepare_v2(
        db,
        "INSERT INTO course(course_code, course_name, professor)"
        "VALUES (?,?,?)",
        -1,
        &stmt,
        NULL
    );
    
    sqlite3_bind_text(stmt, 1, c -> course_id, -1, SQLITE_TRANSIENT); 
    sqlite3_bind_text(stmt, 2, c -> course_name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, c -> professor, -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Insert course failed: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt); 
}




























/////////////////////////////////////END OF COURSE DATABASE FUNCTIONS/////////////////////////////////////