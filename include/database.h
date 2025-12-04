#ifndef DATABASE_H
#define DATABASE_H
#include "sqlite3.h"

typedef struct Student{
    int id;
    char* first_name;
    char* last_name; 
    char* date_of_birth;  
    char* status;         
    Major* major;
    struct Student* next; 
}Student;

typedef struct{
    Student* head;
    int num_of_students; 
}Student_List;

typedef struct{
    Course* head;
    int num_of_courses;
}Course_List;

typedef struct Course{
    char course_id[10]; 
    char course_name[50];
    char professor[50];
    struct Course* next;
}Course;

typedef struct Major{
    char major_id[10]; 
    char major_name[50]; 
    Course* courses;
    int num_of_courses;
    int capacity; 
    int enrolled_students;
    char head_professor[50];
    struct Major* next;
}Major; 


/////////////////////////////////STUDENT DATABASE FUNCTIONS//////////////////////////////////////////
int Insert_student_list(Student_List* list, Student* student); 
Student* CreateNode();
Student_List* CreateList();
void Save_student(sqlite3 *db, Student_List* list, Student* student);
void removeStudent(sqlite3 *db, Student_List* list, int id);
void updateStudent(sqlite3 *db, Student_List* list, int id);
void printList(Student_List* list);
void freeList(Student_List* list);
///////////////////////////////END OF STUDENT DATABASE FUNCTIONS/////////////////////////////////////



////////////////////////////////COURSE DATABASE FUNCTIONS//////////////////////////////////////////
Course* createCourse();
int Insert_course_list(Course_List* list, Course* course);
void SaveCourse(sqlite3 *db, Course_List* list, Course* course);
void add_course_to_major(Major* major, Course* course);
void print_courses_in_major(Major* major);
////////////////////////////////END OF COURSE DATABASE FUNCTIONS/////////////////////////////////////



////////////////////////////////DATABASE OPERATION FUNCTIONS//////////////////////////////////////////
int db_connect(sqlite3 **db, char* db_name);
int CreateStudentTable(sqlite3 *db);
int db_insert_student(sqlite3 *db, Student* s);
void db_remove_student(sqlite3 *db, int id);
void load_students(sqlite3 *db, Student_List* list);
void db_update_student(sqlite3 *db, Student s); 


void CreateCourseTable(sqlite3* db);
void db_insert_course(sqlite3* db, Course* c);
////////////////////////////////END OF DATABASE OPERATION FUNCTIONS/////////////////////////////////////

#endif // DATABASE_H