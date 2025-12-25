#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
      "phone_number TEXT, "
      "email TEXT, "
      "address TEXT, "
      "major_id TEXT, "
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

    if(sqlite3_prepare_v2(
        db,
        "INSERT INTO student(first_name, last_name, date_of_birth,status, phone_number, email, address)"
        "VALUES (?,?,?,?,?,?,?)",
        -1,
        &stmt,
        NULL
    ) != SQLITE_OK){
        printf("Prepare failed: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    sqlite3_bind_text(stmt, 1, s -> first_name, -1, SQLITE_TRANSIENT); 
    sqlite3_bind_text(stmt, 2, s -> last_name, -1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, s -> date_of_birth, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, s -> status, -1, SQLITE_TRANSIENT); 
    sqlite3_bind_text(stmt, 5, s -> phone_number, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, s -> email, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, s -> address, -1, SQLITE_TRANSIENT);
    
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
        const char* phone_number = (const char*) sqlite3_column_text(stmt, 4);
        const char* email = (const char*) sqlite3_column_text(stmt, 5);
        const char* address = (const char*) sqlite3_column_text(stmt, 6);
        const char* major_id = (const char*) sqlite3_column_text(stmt, 7);
        const char* status = (const char*) sqlite3_column_text(stmt, 8);
        Student* temp = malloc(sizeof(Student)); 
        temp -> first_name = strdup(fn);
        temp -> last_name = strdup(ln);
        temp -> date_of_birth = strdup(dob);
        temp -> phone_number = strdup(phone_number);
        temp -> email = strdup(email);
        temp -> address = strdup(address);
        temp -> major_id = strdup(major_id);
        temp -> status = strdup(status);
        temp -> next = NULL;

        temp->id = sqlite3_column_int(stmt, 0);
        Insert_student_list(list, temp);
   }

   sqlite3_finalize(stmt); 
}

void db_remove_student(sqlite3 *db, int id){
    sqlite3_stmt* stmt; 

    if(sqlite3_prepare_v2(
        db,
        "DELETE FROM student WHERE id = ?",
        -1,
        &stmt,
        NULL
    ) != SQLITE_OK){
        printf("Prepare failed: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    sqlite3_bind_int(stmt, 1, id); 

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Delete failed: %s\n", sqlite3_errmsg(db));
    } 
    sqlite3_finalize(stmt); 
}

void db_update_student(sqlite3 *db, Student* s){
    sqlite3_stmt* stmt; 

    if(sqlite3_prepare_v2(
        db,
        "UPDATE student SET first_name = ?, last_name = ?, date_of_birth = ?,major_id = ?, status = ? WHERE id = ?",
        -1,
        &stmt,
        NULL
    ) != SQLITE_OK){
        printf("Prepare failed: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    sqlite3_bind_text(stmt, 1, s -> first_name, -1, SQLITE_TRANSIENT); 
    sqlite3_bind_text(stmt, 2, s -> last_name, -1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, s -> date_of_birth, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, s -> major_id, -1, SQLITE_TRANSIENT); 
    sqlite3_bind_text(stmt, 5, s -> status, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 6, s -> id);
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
                "course_id TEXT NOT NULL, "
                "course_name TEXT NOT NULL, "
                "professor TEXT NOT NULL);";


    if(sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK){
        printf("Create course table failed: %s\n", sqlite3_errmsg(db));
    }
}

int db_insert_course(sqlite3* db, Course* c){
    sqlite3_stmt* stmt; 

    if(sqlite3_prepare_v2(
        db,
        "INSERT INTO course(course_id, course_name, professor)"
        "VALUES (?,?,?)",
        -1,
        &stmt,
        NULL
    ) != SQLITE_OK){
        printf("Prepare failed: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    sqlite3_bind_text(stmt, 1, c -> course_id, -1, SQLITE_TRANSIENT); 
    sqlite3_bind_text(stmt, 2, c -> course_name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, c -> professor, -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Insert course failed: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt); 
    return 1; 
}

int db_remove_course(sqlite3 *db, const char* id){
    sqlite3_stmt* stmt; 
    int rc = sqlite3_prepare_v2(
        db,
        "DELETE FROM course WHERE course_id = ?",
        -1,
        &stmt,
        NULL
    );
    
    if(rc != SQLITE_OK) {
        printf("Prepare failed: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    sqlite3_bind_text(stmt, 1, id, -1, SQLITE_TRANSIENT); 

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Delete failed: %s\n", sqlite3_errmsg(db));
        return 0; 
    } 
    sqlite3_finalize(stmt); 
    return 1;
}

int db_update_course(sqlite3 *db, Course* c){
    sqlite3_stmt* stmt; 

    if(sqlite3_prepare_v2(
        db,
        "UPDATE course SET course_name = ?, professor = ? WHERE course_id = ?",
        -1,
        &stmt,
        NULL
    ) != SQLITE_OK){
        printf("Prepare failed: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    sqlite3_bind_text(stmt, 1, c -> course_name, -1, SQLITE_TRANSIENT); 
    sqlite3_bind_text(stmt, 2, c -> professor, -1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, c -> course_id, -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Update course failed: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt); 
    return 1;
}    

void load_courses(sqlite3 *db, Course_List* list){
   sqlite3_stmt* stmt; 
   
   if (sqlite3_prepare_v2(
        db,
        "SELECT * FROM course",
        -1,
        &stmt,
        NULL) != SQLITE_OK){
            printf("Error preparing SELECT statement: %s\n", sqlite3_errmsg(db));
            return;
   }

   while(sqlite3_step(stmt) == SQLITE_ROW){
        const char* course_id = (const char*) sqlite3_column_text(stmt, 1);
        const char* course_name = (const char*) sqlite3_column_text(stmt, 2);
        const char* professor = (const char*) sqlite3_column_text(stmt, 3);
        Course* temp = malloc(sizeof(Course)); 
        temp -> course_id = strdup(course_id);
        temp -> course_name = strdup(course_name);
        temp -> professor = strdup(professor);
        temp -> next = NULL;

        Insert_course_list(list, temp);
   }

   sqlite3_finalize(stmt); 
}



/////////////////////////////////////END OF COURSE DATABASE FUNCTIONS/////////////////////////////////////

//////////////////////////////////MAJOR FUNCTIONS//////////////////////////////////////////

void CreateMajorTable(sqlite3* db){
    char* sql = "CREATE TABLE IF NOT EXISTS major ("
                "major_id TEXT PRIMARY KEY, "
                "major_name TEXT NOT NULL, "
                "head_professor TEXT NOT NULL,"
                "capacity INTEGER, "
                "enrolled_students INTEGER);"; 
    
    if(sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK){
        printf("Create major table failed: %s\n", sqlite3_errmsg(db));
    }
}

int db_insert_major(sqlite3* db, Major* major){
    sqlite3_stmt* stmt; 

    if(sqlite3_prepare_v2(
        db,
        "INSERT INTO major(major_id, major_name, head_professor, capacity, enrolled_students)"
        "VALUES (?,?,?,?,?)",
        -1,
        &stmt,
        NULL
    ) != SQLITE_OK){
        printf("Prepare failed: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    sqlite3_bind_text(stmt, 1, major -> major_id, -1, SQLITE_TRANSIENT); 
    sqlite3_bind_text(stmt, 2, major -> major_name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, major -> head_professor, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, major -> capacity);
    sqlite3_bind_int(stmt, 5, major -> enrolled_students);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Insert major failed: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt); 

    return 1;
}

int db_remove_major(sqlite3 *db, const char* major_id){
    sqlite3_stmt* stmt; 
    int rc = sqlite3_prepare_v2(
        db,
        "DELETE FROM major WHERE major_id = ?",
        -1,
        &stmt,
        NULL
    );
    
    if(rc != SQLITE_OK) {
        printf("Prepare failed: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    sqlite3_bind_text(stmt, 1, major_id, -1, SQLITE_TRANSIENT); 

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Delete failed: %s\n", sqlite3_errmsg(db));
        return 0; 
    } 
    sqlite3_finalize(stmt); 
    return 1;
}

int db_update_major(sqlite3 *db, Major* major){
    sqlite3_stmt* stmt; 

    if(sqlite3_prepare_v2(
        db,
        "UPDATE major SET major_name = ?, head_professor = ?, capacity = ?, enrolled_students = ? WHERE major_id = ?",
        -1,
        &stmt,
        NULL
    ) != SQLITE_OK){
        printf("Prepare failed: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    sqlite3_bind_text(stmt, 1, major -> major_name, -1, SQLITE_TRANSIENT); 
    sqlite3_bind_text(stmt, 2, major -> head_professor, -1,SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, major -> capacity);
    sqlite3_bind_int(stmt, 4, major -> enrolled_students);
    sqlite3_bind_text(stmt, 5, major -> major_id, -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Update major failed: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);

    return 1;
}

void LoadMajors(sqlite3 *db, Major_List* list){
   sqlite3_stmt* stmt; 
   
   if (sqlite3_prepare_v2(
        db,
        "SELECT * FROM major",
        -1,
        &stmt,
        NULL) != SQLITE_OK){
            printf("Error preparing SELECT statement: %s\n", sqlite3_errmsg(db));
            return;
   }

   while(sqlite3_step(stmt) == SQLITE_ROW){
        const char* major_id = (const char*) sqlite3_column_text(stmt, 0);
        const char* major_name = (const char*) sqlite3_column_text(stmt, 1);
        const char* head_professor = (const char*) sqlite3_column_text(stmt, 2);
        int capacity = sqlite3_column_int(stmt, 3);
        int enrolled_students = sqlite3_column_int(stmt, 4);

        Major* temp = malloc(sizeof(Major)); 
        temp -> major_id = strdup(major_id);
        temp -> major_name = strdup(major_name);
        temp -> head_professor = strdup(head_professor);
        temp -> course_list = CreateCourseList();
        temp -> courses_loaded = 0;
        temp -> capacity = capacity;
        temp -> enrolled_students = enrolled_students;
        temp -> next = NULL;

        Insert_major_list(list, temp);
   }

   sqlite3_finalize(stmt); 
}






























/////////////////////////////////END OF MAJOR FUNCTIONS/////////////////////////////////////

////////////////////////////////MAJOR COURSE FUNCTIONS/////////////////////////////////////

void CreateMajorCourseTable(sqlite3* db){
    char* sql = "CREATE TABLE IF NOT EXISTS major_course ("
                "major_id TEXT NOT NULL, "
                "course_id TEXT NOT NULL, "
                "course_name TEXT NOT NULL,"
                "professor TEXT NOT NULL);"; 
    
    if(sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK){
        printf("Create major_course table failed: %s\n", sqlite3_errmsg(db));
    }
}

int db_insert_major_course(sqlite3* db, const char* major_id, Course* c){
    sqlite3_stmt* stmt; 

    if(sqlite3_prepare_v2(
        db,
        "INSERT INTO major_course(major_id, course_id, course_name, professor)"
        "VALUES (?,?,?,?)",
        -1,
        &stmt,
        NULL
    ) != SQLITE_OK){
        printf("Prepare failed: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    sqlite3_bind_text(stmt, 1, major_id, -1, SQLITE_TRANSIENT); 
    sqlite3_bind_text(stmt, 2, c -> course_id, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, c -> course_name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, c -> professor, -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Insert major_course failed: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);
    return 1;
}

void load_major_courses(sqlite3 *db, Major_List* major_list, char* major_id){

    Major* major = major_list->head;
    while (major && strcmp(major->major_id, major_id) != 0) {
        major = major->next;
    }

    if(major == NULL){
        printf("Major with ID %s not found.\n", major_id);
        return; 
    }

    sqlite3_stmt* stmt; 
   
   if (sqlite3_prepare_v2(
        db,
        "SELECT * FROM major_course WHERE major_id = ?",
        -1,
        &stmt,
        NULL) != SQLITE_OK){
            printf("Error preparing SELECT statement: %s\n", sqlite3_errmsg(db));
            return;
   }

   sqlite3_bind_text(stmt, 1, major_id, -1, SQLITE_TRANSIENT);

   while(sqlite3_step(stmt) == SQLITE_ROW){
        const char* major_id = (const char*) sqlite3_column_text(stmt, 0);
        const char* course_id = (const char*) sqlite3_column_text(stmt, 1);
        const char* course_name = (const char*) sqlite3_column_text(stmt, 2);
        const char* professor = (const char*) sqlite3_column_text(stmt, 3);

        Major* major = major_list -> head;
        while(major != NULL && strcmp(major -> major_id, major_id) != 0){
            major = major -> next; 
        }

        if(major != NULL){
            Course* new_course = malloc(sizeof(Course));
            new_course -> course_id = strdup(course_id);
            new_course -> course_name = strdup(course_name);
            new_course -> professor = strdup(professor);
            new_course -> next = NULL;

            Insert_course_list(major -> course_list, new_course);
        }
   }

   sqlite3_finalize(stmt); 
}
















////////////////////////////////END OF MAJOR COURSE FUNCTIONS/////////////////////////////////