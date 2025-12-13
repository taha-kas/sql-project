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
    char course_id[50];
    fgets(course_id, sizeof(course_id), stdin);
    course_id[strcspn(course_id, "\n")] = '\0';
    new_course->course_id = strdup(course_id);

    //Get Course Name:
    printf("Enter Course Name: ");
    char course_name[100];
    fgets(course_name, sizeof(course_name), stdin); 
    course_name[strcspn(course_name, "\n")] = '\0';
    new_course->course_name = strdup(course_name);

    //Get Professor Name:
    printf("Enter Professor Name: ");
    char professor[100];
    fgets(professor, sizeof(professor), stdin); 
    professor[strcspn(professor, "\n")] = '\0';
    new_course->professor = strdup(professor);
    
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
void add_course_to_major(sqlite3 *db, Major_List* list, char* major_id, char* course_id, char* course_name, char* professor){
    Major* major = list -> head;

    while(major != NULL && strcmp(major -> major_id, major_id) != 0){
        major = major -> next; 
    }

    if(major == NULL){ //Check if major exists
        printf("Major with ID %s not found.\n", major_id);
        return; 
    }
    
    if(major -> course_list == NULL){
        major -> course_list = CreateCourseList();
    }

    Course* exists = major -> course_list -> head;

    while(exists != NULL){
        if(strcmp(exists -> course_id, course_id) == 0){
            printf("Course with ID %s already exists in Major %s.\n", course_id, major_id);
            return; 
        }
        exists = exists -> next; 
    }

    Course* course = malloc(sizeof(Course));
    course->course_id = strdup(course_id);
    course->course_name = strdup(course_name);
    course->professor = strdup(professor);
    course -> next = NULL;


    if(major -> course_list -> head == NULL){
        major -> course_list -> head = course; 
        major -> course_list -> num_of_courses++;
        return; 
    }
    
    Course* temp = major -> course_list -> head; 
    while(temp -> next != NULL){
        temp = temp -> next; 
    }
    temp -> next = course; 
    major -> course_list -> num_of_courses++;

    db_insert_major_course(db, major -> major_id, course);

    return;
}

//A refaire plus tard!!! Passer le code de la filiere comme argument!!!
void print_courses_in_major(Major_List* list, char* major_id){
    Major* temp = list -> head;

    while(temp != NULL && strcmp(temp -> major_id, major_id) != 0){
        temp = temp -> next; 
    }

    if(temp == NULL){
        printf("Major with ID %s not found.\n", major_id);
        return; 
    }

    Course* temp_course = temp -> course_list -> head;
    
    if(temp_course == NULL){
        printf("No courses available in Major %s.\n", major_id);
        return; 
    }

    printf("________________________________________________________________\n");
    printf("Courses in Major %s:\n", major_id);
    printf("________________________________________________________________\n");
    printf("%-15s%-30s%-30s\n", "Course ID", "Course Name", "Professor");
    printf("________________________________________________________________\n");
    
    while(temp_course != NULL){
        printf("%-15s%-30s%-30s\n", temp_course -> course_id, temp_course -> course_name, temp_course -> professor);
        temp_course = temp_course -> next; 
    }
    
    printf("________________________________________________________________\n");

    return; 
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

    printf("What fields do you want to update?\n");

    printf("1. Course Name (y/n)\n");
    char choice[10];
    fgets(choice, sizeof(choice), stdin); 
    if(choice[0] == 'y' || choice[0] == 'Y'){
        printf("Enter new Course Name: ");
        char course_name[100];
        fgets(course_name, sizeof(course_name), stdin);
        course_name[strcspn(course_name, "\n")] = 0;

        //Liberer d'abord l'espace alloué précédemment
        if(temp -> course_name != NULL){
            free(temp -> course_name);
        }

        temp -> course_name = strdup(course_name);
    }

    printf("2. Professor (y/n)\n");
    fgets(choice, sizeof(choice), stdin);
    if(choice[0] == 'y' || choice[0] == 'Y'){
        printf("Enter new Professor Name: ");
        char professor[100];
        fgets(professor, sizeof(professor), stdin);
        professor[strcspn(professor, "\n")] = 0;

        if(temp -> professor != NULL){
            free(temp -> professor);
        }

        temp -> professor = strdup(professor);
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
    printf("%-15s%-30s%-30s\n", "Course ID", "Course Name", "Professor");
    printf("________________________________________________________________\n");
    while(temp != NULL){
        printf("%-15s%-30s%-30s\n", temp -> course_id, temp -> course_name, temp -> professor);
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

int freeCourseList(Course_List* list){
    Course* temp = list -> head; 
    Course* next; 

    while(temp != NULL){
        next = temp -> next; 
        free(temp -> course_id);
        free(temp -> course_name);
        free(temp -> professor);
        free(temp);
        temp = next; 
    }

    //free(list); //Optional: if we want to free the whole list structure and not only its content
    printf("Course list has been freed successfully.\n");
    return 1; 
}