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

Student* CreateNode(char* first_name, char* last_name, char* dob, char* status){
    Student* temp = malloc(sizeof(Student)); 

    if(temp == NULL){
        printf("Error: Could not allocate memory\n"); 
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
//  To correct tomorrow: Insertion from the end instead of the beginning
int Insert_student(Student_List* list, Student* student) {

    if(list == NULL || student == NULL) {
        printf("Error: List or student is NULL\n");
        return 0; 
    }

    else if(list -> head == NULL){
        list -> head = student;
        list -> num_of_students++;
        return 1; 
    }

    Student* temp = list -> head; 
    while(temp -> next != NULL){
        temp = temp -> next; 
    }
    temp -> next = student;
    list -> num_of_students++;
    return 1;
}


void Save_student(sqlite3 *db, Student_List* list, Student* student){
    if(!db_insert_student(db, student)){
        printf("Error: Could not insert student into database\n");
        return; 
    }
    Insert_student(list, student);
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

void removeStudent(sqlite3 *db, Student_List* list, int id){
    Student* temp = list -> head;
    Student* prev = NULL;

    while(temp != NULL && temp -> id != id){
        prev = temp; 
        temp = temp -> next; 
    }

    if(temp == NULL){
        printf("Student with ID %d not found.\n", id);
        return; 
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
    return; 
}

void updateStudent(sqlite3 *db, Student_List* list, int id){
    Student* temp = list -> head; 

    while(temp != NULL && temp -> id != id){
        temp = temp -> next; 
    }

    if(temp == NULL){
        printf("Student with ID %d not found.\n", id);
        return; 
    }

    printf("Current details:\n");
    printf("First Name: %s\n", temp -> first_name);
    printf("Last Name: %s\n", temp -> last_name);
    printf("Date of Birth: %s\n", temp -> date_of_birth);
    printf("Status: %s\n", temp -> status);

    printf("What fields do you want to update?\n");

    printf("1. First Name (y/n)\n");
    char choice;
    scanf(" %c", &choice);
    getchar();    
    if(choice == 'y' || choice == 'Y'){
        char new_first_name[50];
        printf("Enter new First Name: ");
        fgets(new_first_name, sizeof(new_first_name), stdin);
        new_first_name[strcspn(new_first_name, "\n")] = 0;
        free(temp -> first_name);
        temp -> first_name = strdup(new_first_name);
    }

    printf("2. Last Name (y/n)\n");
    scanf(" %c", &choice);
    getchar();
    if(choice == 'y' || choice == 'Y'){
        char new_last_name[50];
        printf("Enter new Last Name: ");
        fgets(new_last_name, sizeof(new_last_name), stdin);
        new_last_name[strcspn(new_last_name, "\n")] = 0;
        free(temp -> last_name);
        temp -> last_name = strdup(new_last_name);
    }
    
    printf("3. Date of Birth (y/n)\n");
    scanf(" %c", &choice);
    getchar();
    if(choice == 'y' || choice == 'Y'){
        char new_dob[11];
        printf("Enter new Date of Birth (YYYY-MM-DD): ");
        fgets(new_dob, sizeof(new_dob), stdin);
        new_dob[strcspn(new_dob, "\n")] = 0;
        free(temp -> date_of_birth);
        temp -> date_of_birth = strdup(new_dob);
    }

    printf("4. Status (y/n)\n");
    scanf(" %c", &choice);
    getchar();
    if(choice == 'y' || choice == 'Y'){
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

    db_update_student(db, *temp); // You need to pass the database connection here

    return; 
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
    sqlite3 *test;
    char *err_msg = 0;

    db_connect(&test, "test.db");

    // Create a table
    db_create_table(test); 

    // Query and display the data
    Student_List* list = CreateList();
    Save_student(test, list, CreateNode("John", "Doe", "2000-01-01", "active"));
    Save_student(test, list, CreateNode("Jane", "Smith", "1999-05-15", "pending"));   
    Save_student(test, list, CreateNode("Alice", "Johnson", "2001-09-23", "pending"));
    Save_student(test, list, CreateNode("Bob", "Brown", "1998-12-30", "pending"));
    Save_student(test, list, CreateNode("Charlie", "Davis", "2002-07-11", "pending"));
    Save_student(test, list, CreateNode("Eve", "Wilson", "2000-03-05", "pending"));

    // removeStudent(test, list, 4);

    // updateStudent(test, list, 2);

    // printList(list);

    // load_students(test, list);
    // Save_student(test, list, CreateNode("Diana", "Prince", "1995-04-25", "active"));
    // removeStudent(test, list, 8);

    printList(list);
    

    sqlite3_close(test);


    printf("\nDatabase operations completed successfully!\n");
    printf("Check your folder for 'test.db' file\n");


    return 0;
}