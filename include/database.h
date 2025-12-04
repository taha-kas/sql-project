#ifndef DATABASE_H
#define DATABASE_H
#include "sqlite3.h"

typedef struct Student{
    int id;
    char* first_name;
    char* last_name; 
    char* date_of_birth;  // YYYY-MM-DD
    char* status;         // pending/active/graduated/dropped
    struct Student* next; 
}Student;

typedef struct{
    Student* head;
    int num_of_students; 
}Student_List;



int Insert_student(Student_List* list, Student* student); 
Student* CreateNode(char* first_name, char* last_name, char* dob, char* status);
Student_List* CreateList();
void Save_student(sqlite3 *db, Student_List* list, Student* student);
void removeStudent(sqlite3 *db, Student_List* list, int id);
void updateStudent(sqlite3 *db, Student_List* list, int id);
void printList(Student_List* list);
void freeList(Student_List* list);



int db_connect(sqlite3 **db, char* db_name);
int db_create_table(sqlite3 *db);
int db_insert_student(sqlite3 *db, Student* s);
void db_remove_student(sqlite3 *db, int id);
void load_students(sqlite3 *db, Student_List* list);
void db_update_student(sqlite3 *db, Student s); 

#endif // DATABASE_H