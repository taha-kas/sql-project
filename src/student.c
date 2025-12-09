#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include "database.h"



Student_List* CreateList(){
    Student_List* temp = malloc(sizeof(Student_List)); 

    if(temp == NULL){
        printf("Error: Could not allocate memory\n"); 
        return NULL; 
    }

    temp -> head = NULL; 
    temp -> num_of_students = 0; 

    return temp; 
}

Student* CreateNode(){

    //Get First Name:
    printf("Enter First Name: ");
    char first_name[50];
    fgets(first_name, sizeof(first_name), stdin); 
    first_name[strcspn(first_name, "\n")] = '\0'; 

    //Get Last Name:
    printf("Enter Last Name: ");
    char last_name[50];
    fgets(last_name, sizeof(last_name), stdin); 
    last_name[strcspn(last_name, "\n")] = '\0'; 

    //Get Date of Birth:
    printf("Enter Date of Birth (YYYY-MM-DD): "); 
    char dob[12];
    fgets(dob, sizeof(dob), stdin);
    dob[strcspn(dob, "\n")] = '\0';

    //Get Status:
    printf("Enter Status (pending/active/graduated/dropped): ");
    char status[20]; 
    fgets(status, sizeof(status), stdin); 
    status[strcspn(status, "\n")] = '\0'; 

    //Check for valid status:
    while(strcmp(status, "pending") != 0 && strcmp(status, "active") != 0 && strcmp(status, "graduated") != 0 && strcmp(status, "dropped") != 0){
        printf("Invalid status. Please enter one of the following: pending, active, graduated, dropped\n");
        fgets(status, sizeof(status), stdin);
        status[strcspn(status, "\n")] = '\0'; 
    }

    Student* temp = malloc(sizeof(Student)); 

    if(temp == NULL){
        printf("Error: Could not allocate memory. Failed to create student.\n"); 
        return NULL; 
    }

    temp -> id = 0; 
    temp -> first_name = strdup(first_name);
    temp -> last_name = strdup(last_name);
    temp -> date_of_birth = strdup(dob);
    temp -> status = strdup(status);
    temp -> next = NULL;


    return temp;
}

int Insert_student_list(Student_List* list, Student* student) {

    if(list == NULL || student == NULL) {
        printf("Error: List or student is NULL\n");
        return 0; 
    }

    else if(list -> head == NULL){
        list -> head = student;
        list -> num_of_students++;
        printf("Student with ID %d added successfully.\n", student -> id);
        return 1; 
    }

    Student* temp = list -> head; 
    while(temp -> next != NULL){
        temp = temp -> next; 
    }
    temp -> next = student;
    list -> num_of_students++;
    printf("Student with ID %d added successfully.\n", student -> id);
    return 1;
}


int Save_student(sqlite3 *db, Student_List* list, Student* student){

    if(!db_insert_student(db, student)){
        printf("Error: Could not insert student into database\n");
        return 0; 
    }

    if(!Insert_student_list(list, student)){
        printf("Error: Could not insert student into list\n");
        db_remove_student(db, student->id);
        return 0;
    }

    return 1;
}

void printStudent(Student_List* list, int id){
    Student* temp = list -> head;
    while(temp != NULL){
        if(temp -> id == id){
            printf("Student Details:\n");
            printf("ID: %d\n", temp -> id);
            printf("First Name: %s\n", temp -> first_name);
            printf("Last Name: %s\n", temp -> last_name);
            printf("Date of Birth: %s\n", temp -> date_of_birth);
            printf("Status: %s\n", temp -> status);
            return;
        }
        temp = temp -> next; 
    }
}

void printList(Student_List* list){
    Student* temp = list -> head; 

    printf("Number of students: %d\n\n", list -> num_of_students);
    printf("________________________________________________________________\n");
    printf("%-5s%-15s%-15s%-12s%-10s\n", "ID", "First Name", "Last Name", "DOB", "Status");
    printf("________________________________________________________________\n");
    while(temp != NULL){
        printf("%-5d%-15s%-15s%-12s%-10s\n", temp -> id, temp -> first_name, temp -> last_name, temp -> date_of_birth, temp -> status);
        temp = temp -> next; 
    }
    printf("________________________________________________________________\n");
}

int removeStudent(sqlite3 *db, Student_List* list, int id){
    Student* temp = list -> head;
    Student* prev = NULL;

    while(temp != NULL && temp -> id != id){
        prev = temp; 
        temp = temp -> next; 
    }

    if(temp == NULL){
        printf("Student with ID %d not found.\n", id);
        return 0; 
    }

    if(prev == NULL){
        list -> head = temp -> next; 
    } 
    else{
        prev -> next = temp -> next; 
    }
    free(temp -> first_name);
    free(temp -> last_name);
    free(temp -> date_of_birth);
    free(temp -> status);
    free(temp);
    list -> num_of_students--;

    printf("Student with ID %d removed successfully.\n", id);

    db_remove_student(db, id);
    return 1; 
}

int updateStudent(sqlite3 *db, Student_List* list, int id){
    Student* temp = list -> head; 

    while(temp != NULL && temp -> id != id){
        temp = temp -> next; 
    }

    if(temp == NULL){
        printf("Student with ID %d not found.\n", id);
        return 0; 
    }

    printf("Current details:\n");
    printf("First Name: %s\n", temp -> first_name);
    printf("Last Name: %s\n", temp -> last_name);
    printf("Date of Birth: %s\n", temp -> date_of_birth);
    printf("Status: %s\n", temp -> status);

    printf("What fields do you want to update?\n");

    printf("1. First Name (y/n)\n");
    char choice[10];
    fgets(choice, sizeof(choice), stdin);
    choice[strcspn(choice, "\n")] = 0;

    if(choice[0] == 'y' || choice[0] == 'Y'){
        char new_first_name[50];
        printf("Enter new First Name: ");
        fgets(new_first_name, sizeof(new_first_name), stdin);
        new_first_name[strcspn(new_first_name, "\n")] = 0;
        free(temp -> first_name);
        temp -> first_name = strdup(new_first_name);
    }

    printf("2. Last Name (y/n)\n");
    fgets(choice, sizeof(choice), stdin);
    choice[strcspn(choice, "\n")] = 0;
    if(choice[0] == 'y' || choice[0] == 'Y'){
        char new_last_name[50];
        printf("Enter new Last Name: ");
        fgets(new_last_name, sizeof(new_last_name), stdin);
        new_last_name[strcspn(new_last_name, "\n")] = 0;
        free(temp -> last_name);
        temp -> last_name = strdup(new_last_name);
    }
    
    printf("3. Date of Birth (y/n)\n");
    fgets(choice, sizeof(choice), stdin);
    choice[strcspn(choice, "\n")] = 0;
    if(choice[0] == 'y' || choice[0] == 'Y'){
        char new_dob[11];
        printf("Enter new Date of Birth (YYYY-MM-DD): ");
        fgets(new_dob, sizeof(new_dob), stdin);
        new_dob[strcspn(new_dob, "\n")] = 0;
        free(temp -> date_of_birth);
        temp -> date_of_birth = strdup(new_dob);
    }

    printf("4. Status (y/n)\n");
    fgets(choice, sizeof(choice), stdin);
    choice[strcspn(choice, "\n")] = 0;
    if(choice[0] == 'y' || choice[0] == 'Y'){
        char new_status[20];
        printf("Enter new Status: ");
        fgets(new_status, sizeof(new_status), stdin);
        new_status[strcspn(new_status, "\n")] = 0;
        while(strcmp(new_status, "pending") != 0 && strcmp(new_status, "active") != 0 && strcmp(new_status, "graduated") != 0 && strcmp(new_status, "dropped") != 0){
            printf("Invalid status. Please enter one of the following: pending, active, graduated, dropped\n");
            fgets(new_status, sizeof(new_status), stdin);
            new_status[strcspn(new_status, "\n")] = 0;
        }
        free(temp -> status);
        temp -> status = strdup(new_status);
    }

    printf("Student with ID %d updated successfully.\n", id);

    db_update_student(db, temp); // You need to pass the database connection here

    return 1; 
}

void freeList(Student_List* list){
    Student* temp = list -> head; 
    Student* next; 

    while(temp != NULL){
        next = temp -> next; 
        free(temp -> first_name);
        free(temp -> last_name);
        free(temp -> date_of_birth);
        free(temp -> status);
        free(temp);
        temp = next; 
    }

    free(list);
    printf("List had been freed successfully.\n");
}

int main(){

    printf("Enter the password: ");
    char password[50]; 
    fgets(password, sizeof(password), stdin); 
    password[strcspn(password, "\n")] = '\0';

    if(strcmp(password, "admin123") != 0){
        printf("Error: Incorrect password\n");
        return 1; 
    }

    printf("Password accepted. Access granted.\n");

    sqlite3 *test;
    char *err_msg = 0;

    db_connect(&test, "test.db");

    // Create a table
    CreateStudentTable(test); 

    // Query and display the data
    Student_List* list = CreateList();
    
    // printf("How many students do you want to add: ");
    // int num; 
    // scanf("%d", &num);
    // getchar();

    // for(int i = 0; i < num; i++){
    //     Student* new_student = CreateNode();
    //     if(new_student != NULL){
    //         Save_student(test, list, new_student);
    //     } else {
    //         printf("Failed to create student. Skipping...\n");
    //     }
    // }

    load_students(test, list);

    // removeStudent(test, list, 7);

    updateStudent(test, list, 9);

    // load_students(test, list);


    // removeStudent(test, list, 8);

    printList(list);
    

    sqlite3_close(test);


    printf("\nDatabase operations completed successfully!\n");
    printf("Check your folder for 'test.db' file\n");


    return 0;
}