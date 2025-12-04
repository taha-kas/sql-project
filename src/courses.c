#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include "database.h"


Course* createCourse(){

    //Get Course ID:
    printf("Enter Course ID: ");
    char course_id[10];
    fgets(course_id, sizeof(course_id), stdin); 
    course_id[strcspn(course_id, "\n")] = '\0';

    //Get Course Name:
    printf("Enter Course Name: ");
    char course_name[50];
    fgets(course_name, sizeof(course_name), stdin); 
    course_name[strcspn(course_name, "\n")] = '\0';

    //Get Professor Name:
    printf("Enter Professor Name: ");
    char professor[50];
    fgets(professor, sizeof(professor), stdin);
    professor[strcspn(professor, "\n")] = '\0';

    //Get Semester:
    printf("Enter Semester (e.g., Fall, Spring): ");
    char semester[5];
    fgets(semester, sizeof(semester), stdin);
    semester[strcspn(semester, "\n")] = '\0';

    Course* new_course = malloc(sizeof(Course));
    if(new_course == NULL){
        printf("Error: Could not allocate memory for new course.\n");
        return NULL;
    }

    strncpy(new_course->course_id, course_id, sizeof(new_course->course_id) - 1);
    strncpy(new_course->course_name, course_name, sizeof(new_course->course_name) - 1);
    strncpy(new_course->professor, professor, sizeof(new_course->professor) - 1);
    new_course->next = NULL;

    return new_course;
}

int Insert_course_list(Course_List* list, Course* course) {

    if(list == NULL || course == NULL) {
        printf("Error: List or course is NULL\n");
        return 0; 
    }

    else if(list -> head == NULL){
        list -> head = course;
        list -> num_of_courses++;
        return 1; 
    }

    Course* temp = list -> head; 
    while(temp -> next != NULL){
        temp = temp -> next; 
    }
    temp -> next = course;
    list -> num_of_courses++;
    return 1;
}

void SaveCourse(sqlite3 *db, Course_List* list, Course* course){

    if(!db_insert_course(db, course)){
        printf("Error: Could not insert course into database\n");
        return; 
    }

    Insert_course_list(list, course);
}

void add_course_to_major(Major* major, Course* course){
    if(major -> courses == NULL){
        major -> courses = course; 
        return; 
    }
    
    Course* temp = major -> courses; 
    while(temp -> next != NULL){
        temp = temp -> next; 
    }
    temp -> next = course; 
    return;
}

void print_courses_in_major(Major* major){
    Course* temp = major -> courses; 
    printf("________________________________________________________________\n");
    printf("Courses in Major %s:\n", major -> major_name);
    printf("________________________________________________________________\n");
    while(temp != NULL){
        printf("Course ID: %-15s, Course Name: %-30s, Professor: %-20s\n", temp -> course_id, temp -> course_name, temp -> professor);
        temp = temp -> next; 
    }
}

void updateCourse(Course_List* list, char* course_id){
    Course* temp = list -> head; 

    while(temp != NULL && strcmp(temp -> course_id, course_id) != 0){
        temp = temp -> next; 
    }

    if(temp == NULL){
        printf("Course with ID %s not found.\n", course_id);
        return; 
    }

    printf("Current details:\n");
    printf("Course ID: %s\n", temp -> course_id);
    printf("Course Name: %s\n", temp -> course_name);
    printf("Professor: %s\n", temp -> professor);

    printf("What fields do you want to update?\n");

    printf("1. Course Name (y/n)\n");
    char choice;
    scanf(" %c", &choice);
    getchar();    
    if(choice == 'y' || choice == 'Y'){
        char new_course_name[50];
        printf("Enter new Course Name: ");
        fgets(new_course_name, sizeof(new_course_name), stdin);
        new_course_name[strcspn(new_course_name, "\n")] = 0;
        strncpy(temp -> course_name, new_course_name, sizeof(temp -> course_name) - 1);
    }

    printf("2. Professor (y/n)\n");
    scanf(" %c", &choice);
    getchar();    
    if(choice == 'y' || choice == 'Y'){
        char new_professor[50];
        printf("Enter new Professor Name: ");
        fgets(new_professor, sizeof(new_professor), stdin);
        new_professor[strcspn(new_professor, "\n")] = 0;
        strncpy(temp -> professor, new_professor, sizeof(temp -> professor) - 1);
    }

    printf("Course with ID %s updated successfully.\n", course_id);

    return;

}







void test(sqlite3* db, Student s){
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