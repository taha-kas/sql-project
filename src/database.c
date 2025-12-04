#include <stdio.h>
#include <string.h>
#include "sqlite3.h"
#include "database.h"


int db_connect(sqlite3 **db, char* db_name){
    if (sqlite3_open(db_name, db) != SQLITE_OK) {
        printf("Error opening DB: %s\n", sqlite3_errmsg(*db));
        return 0;
    }
    printf("Database opened successfully!\n"); 
    return 1;
}


int db_create_table(sqlite3 *db){
char sql[512] = "CREATE TABLE IF NOT EXISTS student ("
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

// void load_students(sqlite3 *db, Student** head){
//     sqlite3_stmt* stmt; 

//     sqlite3_prepare_v2(
//         db,
//         "SELECT first_name, last_name, date_of_birth, status FROM student",
//         -1,
//         &stmt,
//         NULL
//     );

//     while (sqlite3_step(stmt) == SQLITE_ROW) {
//         Student* new_student = malloc(sizeof(Student));
//         new_student->first_name = strdup((const char*)sqlite3_column_text(stmt, 0));
//         new_student->last_name = strdup((const char*)sqlite3_column_text(stmt, 1));
//         new_student->date_of_birth = strdup((const char*)sqlite3_column_text(stmt, 2));
//         new_student->status = strdup((const char*)sqlite3_column_text(stmt, 3));
//         new_student->next = *head;
//         *head = new_student;
//     }

//     sqlite3_finalize(stmt);
// }

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

        Student* temp = CreateNode(
        fn ? (char*)fn : "", //In case of NULL values, strdup would fail (segmentation fault), we do a check here to assign empty strings instead
        ln ? (char*)ln : "", 
        dob ? (char*)dob : "",
        status ? (char*)status : ""
        );

        temp->id = sqlite3_column_int(stmt, 0);
        Insert_student(list, temp);
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