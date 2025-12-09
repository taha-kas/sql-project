#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include "database.h"


Course* createCourse(){
    Course* new_course = malloc(sizeof(Course));

    if(new_course == NULL){
        printf("Error: Could not allocate memory for new course.\n");
        return NULL;
    }

    //Get Course ID:
    printf("Enter Course ID: ");
    fgets(new_course->course_id, sizeof(new_course->course_id), stdin); 
    new_course->course_id[strcspn(new_course->course_id, "\n")] = '\0';

    //Get Course Name:
    printf("Enter Course Name: ");
    fgets(new_course->course_name, sizeof(new_course->course_name), stdin); 
    new_course->course_name[strcspn(new_course->course_name, "\n")] = '\0';

    //Get Professor Name:
    printf("Enter Professor Name: ");
    fgets(new_course->professor, sizeof(new_course->professor), stdin);
    new_course->professor[strcspn(new_course->professor, "\n")] = '\0';
    
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

int SaveCourse(sqlite3 *db, Course_List* list, Course* course){

    if(!db_insert_course(db, course)){
        printf("Error: Could not insert course into database\n");
        return 0; 
    }

    if(!Insert_course_list(list, course)){
        printf("Error: Could not insert course into list\n");
        db_remove_course(db, course->course_id);
        return 0;
    }
    return 1;
}

//Might need to pass db connection later
void add_course_to_major(sqlite3 *db, Major* major, Course* course){
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
    printf("%-15s%-30s%-20s\n", "Course ID", "Course Name", "Professor");
    printf("________________________________________________________________\n");
    while(temp != NULL){
        printf("%-15s%-30s%-20s\n", temp -> course_id, temp -> course_name, temp -> professor);
        temp = temp -> next; 
    }
}

int updateCourse(sqlite3 *db, Course_List* list, char* course_id){
    Course* temp = list -> head; 

    while(temp != NULL && strcmp(temp -> course_id, course_id) != 0){
        temp = temp -> next; 
    }

    if(temp == NULL){
        printf("Course with ID %s not found.\n", course_id);
        return 0; 
    }

    printf("Current details:\n");
    printf("Course ID: %s\n", temp -> course_id);
    printf("Course Name: %s\n", temp -> course_name);
    printf("Professor: %s\n", temp -> professor);

    char c; 
    while((c = getchar()) != '\n' && c != EOF); 

    printf("What fields do you want to update?\n");

    printf("1. Course Name (y/n)\n");
    char choice[10];
    fgets(choice, sizeof(choice), stdin); 
    if(choice[0] == 'y' || choice[0] == 'Y'){
        printf("Enter new Course Name: ");
        fgets(temp -> course_name, sizeof(temp -> course_name), stdin);
        temp -> course_name[strcspn(temp -> course_name, "\n")] = 0;
    }

    printf("2. Professor (y/n)\n");
    fgets(choice, sizeof(choice), stdin); 
    if(choice[0] == 'y' || choice[0] == 'Y'){
        printf("Enter new Professor Name: ");
        fgets(temp -> professor, sizeof(temp -> professor), stdin);
        temp -> professor[strcspn(temp -> professor, "\n")] = 0;
    }

    if(!db_update_course(db, temp)){
        printf("Error: Could not update course in database\n");
        return 0; 
    }

    printf("Course with ID %s has been updated successfully.\n", course_id);

    return 1;

}

int removeCourse(sqlite3 *db, Course_List* list, char* course_id){
    Course* temp = list -> head; 
    Course* prev = NULL; 

    while(temp != NULL && strcmp(course_id, temp -> course_id) != 0){
        prev = temp; 
        temp = temp -> next;
    }

    if(temp == NULL){
        printf("Course with ID %s not found.\n", course_id);
        return 0; 
    }

    if(prev == NULL){
        list -> head = temp -> next;
        free(temp);
    }

    else{
        prev -> next = temp -> next; 
        free(temp);
    }

    if(!db_remove_course(db, course_id)){
        printf("Error: Could not remove course from database\n");
        return 0; 
    }

    list -> num_of_courses--;
    printf("Course with ID %s has been removed successfully.\n", course_id);

    return 1;
}

int printAllCourses(Course_List* list){
    Course* temp = list -> head; 

    if(temp == NULL){
        printf("No courses available.\n");
        return 0; 
    }

    printf("________________________________________________________________\n");
    printf("All Courses:\n");
    printf("________________________________________________________________\n");
    printf("%-15s%-30s%-20s\n", "Course ID", "Course Name", "Professor");
    printf("________________________________________________________________\n");
    while(temp != NULL){
        printf("%-15s%-30s%-20s\n", temp -> course_id, temp -> course_name, temp -> professor);
        temp = temp -> next; 
    }
    return 1;
}

Course_List* CreateCourseList(){
    Course_List* list = malloc(sizeof(Course_List));
    if(list == NULL){
        printf("Error: Could not allocate memory for course list.\n");
        return NULL;
    }
    list -> head = NULL;
    list -> num_of_courses = 0;
    return list;
}