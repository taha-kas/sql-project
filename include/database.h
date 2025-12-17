#ifndef DATABASE_H
#define DATABASE_H
#include "sqlite3.h"

typedef struct Course{
    char* course_id; 
    char* course_name;
    char* professor;
    struct Course* next;
}Course;

typedef struct{
    Course* head;   
    int num_of_courses;
}Course_List;

typedef struct Major{
    char* major_id; 
    char* major_name;
    Course_List* course_list;
    int capacity; 
    int enrolled_students;
    char* head_professor;
    struct Major* next;
}Major; 

typedef struct{
    Major* head;
    int num_of_majors;
}Major_List;

// typedef struct Major_Course{
//     char* major_id;
//     char** course_ids;
//     char** course_names;
// } Major_Course;

// typedef struct{
//     Major_Course* head;
//     int num_of_courses;
// } Major_Course_List;

typedef struct Student{
    int id;
    char* first_name; //Utiliser des pointeurs au lieu de tableaux pour permettre la duplication de chaînes
    char* last_name; 
    char* date_of_birth;  
    char* status;         
    char* major_id;
    struct Student* next; 
}Student;

typedef struct{
    Student* head;
    int num_of_students; 
}Student_List;




/////////////////////////////////STUDENT FUNCTIONS//////////////////////////////////////////
Student* CreateNode();
Student_List* CreateList();
int Insert_student_list(Student_List* list, Student* student); 
int Save_student(sqlite3 *db, Student_List* list, Student* student);
int updateStudent(sqlite3 *db, Student_List* list, int id);
int removeStudent(sqlite3 *db, Student_List* list, int id);
void printStudent(Student_List* list, int id);
void printStudentList(Student_List* list, Major_List* major_list, char* major_id);
void freeList(Student_List* list);
void importFromCSV(sqlite3 *db, Student_List* list, const char* filename);
///////////////////////////////END OF STUDENT FUNCTIONS/////////////////////////////////////



////////////////////////////////COURSES FUNCTIONS//////////////////////////////////////////
Course* createCourse();
Course_List* CreateCourseList(); 
int Insert_course_list(Course_List* list, Course* course);
int SaveCourse(sqlite3 *db, Course_List* list, Course* course);
void add_course_to_major(sqlite3 *db, Major_List* list, char* major_id, char* course_id, char* course_name, char* professor);
int updateCourse(sqlite3 *db, Course_List* list, char* course_id);
int removeCourse(sqlite3 *db, Course_List* list, char* course_id);
void print_courses_in_major(Major_List* list, char* major_id);
int printAllCourses(Course_List* list);
int freeCourseList(Course_List* list);
////////////////////////////////END OF COURSES FUNCTIONS/////////////////////////////////////



//////////////////////////////MAJOR FUNCTIONS//////////////////////////////////////////
Major* CreateMajor();
Major_List* CreateMajorList();
int Insert_major_list(Major_List* list, Major* major);
int SaveMajor(sqlite3 *db, Major_List* list, Major* major);
int removeMajor(sqlite3 *db, Major_List* list, char* major_id);
int UpdateMajor(sqlite3 *db, Major_List* list, char* major_id);
int printAllMajors(Major_List* list);
int printMajor(Major_List* list, char* major_id);

////////////////////////////////END OF MAJOR FUNCTIONS/////////////////////////////////////



////////////////////////////////DATABASE OPERATION FUNCTIONS//////////////////////////////////////////
int db_connect(sqlite3 **db, char* db_name);
int CreateStudentTable(sqlite3 *db);
int db_insert_student(sqlite3 *db, Student* s);
void db_remove_student(sqlite3 *db, int id);
void load_students(sqlite3 *db, Student_List* list);
void db_update_student(sqlite3 *db, Student* s); 


void CreateCourseTable(sqlite3* db);
int db_insert_course(sqlite3* db, Course* c);
int db_remove_course(sqlite3 *db, const char* id);
int db_update_course(sqlite3 *db, Course* c);
void load_courses(sqlite3 *db, Course_List* list);


void CreateMajorTable(sqlite3* db);
int db_insert_major(sqlite3* db, Major* major);
int db_remove_major(sqlite3 *db, const char* major_id);
int db_update_major(sqlite3 *db, Major* major); 
void LoadMajors(sqlite3 *db, Major_List* major_list);

void CreateMajorCourseTable(sqlite3* db);
int db_insert_major_course(sqlite3* db, const char* major_id, Course* c);
void load_major_courses(sqlite3 *db, Major_List* major_list);
////////////////////////////////END OF DATABASE OPERATION FUNCTIONS/////////////////////////////////////

#endif // DATABASE_H