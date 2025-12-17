#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include "database.h"
#include "ui_helpers.h"



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

    // //Get Status:
    // printf("Enter Status (pending/active/graduated/dropped): ");
    // char status[20]; 
    // fgets(status, sizeof(status), stdin); 
    // status[strcspn(status, "\n")] = '\0'; 

    // //Check for valid status:
    // while(strcmp(status, "pending") != 0 && strcmp(status, "active") != 0 && strcmp(status, "graduated") != 0 && strcmp(status, "dropped") != 0){
    //     printf("Invalid status. Please enter one of the following: pending, active, graduated, dropped\n");
    //     fgets(status, sizeof(status), stdin);
    //     status[strcspn(status, "\n")] = '\0'; 
    // }

    Student* temp = malloc(sizeof(Student)); 

    if(temp == NULL){
        printf("Error: Could not allocate memory. Failed to create student.\n"); 
        return NULL; 
    }

    temp -> id = 0; 
    temp -> first_name = strdup(first_name);
    temp -> last_name = strdup(last_name);
    temp -> date_of_birth = strdup(dob);
    temp -> status = strdup("pending");
    temp -> next = NULL;


    return temp;
}

void importFromCSV(sqlite3 *db, Student_List* list, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("%s%s Could not open file%s\n", COLOR_RED, ICON_ERROR, COLOR_RESET);
        printf("File: %s\n", filename);
        increment_error_count();
        return;
    }

    char line[256];
    int line_number = 0;
    int imported_count = 0;
    int error_count = 0;
    
    printf("%s%s Starting CSV import...%s\n", COLOR_CYAN, ICON_INFO, COLOR_RESET);
    printf("File: %s\n", filename);
    
    while (fgets(line, sizeof(line), file)) {
        line_number++;
        
        // Remove newline character
        line[strcspn(line, "\n")] = '\0';
        
        // Skip empty lines
        if (strlen(line) == 0) {
            printf("%s%s Line %d: Empty line - skipped%s\n", 
                   COLOR_YELLOW, ICON_WARNING, line_number, COLOR_RESET);
            continue;
        }
        
        // Check 1: Count commas to verify field count
        int comma_count = 0;
        for (int i = 0; line[i]; i++) {
            if (line[i] == ',') comma_count++;
        }
        
        // Expected: 2 commas for 3 fields (first_name,last_name,dob)
        if (comma_count != 2) {
            printf("%s%s Line %d: Invalid format - expected 3 fields, found %d%s\n", 
                   COLOR_RED, ICON_ERROR, line_number, comma_count + 1, COLOR_RESET);
            printf("  Line: %s\n", line);
            error_count++;
            increment_error_count();
            continue;
        }
        
        // Parse with validation
        char* first_name = NULL;
        char* last_name = NULL;
        char* dob = NULL;
        
        // Parse first field
        char* token = strtok(line, ",");
        if (token != NULL) {
            first_name = token;
            
            // Check 2: First name not empty
            if (strlen(first_name) == 0) {
                printf("%s%s Line %d: First name cannot be empty%s\n", 
                       COLOR_RED, ICON_ERROR, line_number, COLOR_RESET);
                error_count++;
                increment_error_count();
                continue;
            }
            
            // Check 3: First name length
            if (strlen(first_name) > 50) {
                printf("%s%s Line %d: First name too long (truncated)%s\n", 
                       COLOR_YELLOW, ICON_WARNING, line_number, COLOR_RESET);
                first_name[50] = '\0'; 
            }
        } else {
            printf("%s%s Line %d: Missing first name%s\n", 
                   COLOR_RED, ICON_ERROR, line_number, COLOR_RESET);
            error_count++;
            increment_error_count();
            continue;
        }
        
        // Parse second field
        token = strtok(NULL, ",");
        if (token != NULL) {
            last_name = token;
            
            // Check 4: Last name not empty
            if (strlen(last_name) == 0) {
                printf("%s%s Line %d: Last name cannot be empty%s\n", 
                       COLOR_RED, ICON_ERROR, line_number, COLOR_RESET);
                error_count++;
                increment_error_count();
                continue;
            }
            
            // Check 5: Last name length
            if (strlen(last_name) > 50) {
                printf("%s%s Line %d: Last name too long (truncated)%s\n", 
                       COLOR_YELLOW, ICON_WARNING, line_number, COLOR_RESET);
                last_name[50] = '\0'; 
            }
        } else {
            printf("%s%s Line %d: Missing last name%s\n", 
                   COLOR_RED, ICON_ERROR, line_number, COLOR_RESET);
            error_count++;
            increment_error_count();
            continue;
        }
        
        // Parse third field
        token = strtok(NULL, ",");
        if (token != NULL) {
            dob = token;
            
            // Check 6: Date of birth not empty
            if (strlen(dob) == 0) {
                printf("%s%s Line %d: Date of birth cannot be empty%s\n", 
                       COLOR_RED, ICON_ERROR, line_number, COLOR_RESET);
                error_count++;
                increment_error_count();
                continue;
            }
            
            // Check 7: Validate date format (basic)
            int valid_date = 1;
            if (strlen(dob) != 10) {
                valid_date = 0;
            } else if (dob[4] != '-' || dob[7] != '-') {
                valid_date = 0;
            } else {
                // Check if year, month, day positions are digits
                for (int i = 0; i < 10; i++) {
                    if (i == 4 || i == 7) continue; // Skip hyphens
                    if (dob[i] < '0' || dob[i] > '9') {
                        valid_date = 0;
                        break;
                    }
                }
            }
            
            if (!valid_date) {
                printf("%s%s Line %d: Invalid date format (expected YYYY-MM-DD)%s\n", 
                       COLOR_RED, ICON_ERROR, line_number, COLOR_RESET);
                printf("  Date: %s\n", dob);
                error_count++;
                increment_error_count();
                continue;
            }
            
            // Check 8: Validate date ranges
            int year, month, day;
            if (sscanf(dob, "%d-%d-%d", &year, &month, &day) == 3) {
                if (year < 1900 || year > 2100) {
                    printf("%s%s Line %d: Invalid year (1900-2100)%s\n", 
                           COLOR_RED, ICON_ERROR, line_number, COLOR_RESET);
                    error_count++;
                    increment_error_count();
                    continue;
                }
                if (month < 1 || month > 12) {
                    printf("%s%s Line %d: Invalid month (1-12)%s\n", 
                           COLOR_RED, ICON_ERROR, line_number, COLOR_RESET);
                    error_count++;
                    increment_error_count();
                    continue;
                }
                if (day < 1 || day > 31) {
                    printf("%s%s Line %d: Invalid day (1-31)%s\n", 
                           COLOR_RED, ICON_ERROR, line_number, COLOR_RESET);
                    error_count++;
                    increment_error_count();
                    continue;
                }
            }
        } else {
            printf("%s%s Line %d: Missing date of birth%s\n", 
                   COLOR_RED, ICON_ERROR, line_number, COLOR_RESET);
            error_count++;
            increment_error_count();
            continue;
        }
        
        // Check 9: Check for extra fields (should be nothing left)
        token = strtok(NULL, ",");
        if (token != NULL) {
            printf("%s%s Line %d: Extra fields detected (ignored)%s\n", 
                   COLOR_YELLOW, ICON_WARNING, line_number, COLOR_RESET);
        }
        
        // All checks passed - create student
        Student* new_student = malloc(sizeof(Student));
        if (new_student == NULL) {
            printf("%s%s Line %d: Memory allocation failed%s\n", 
                   COLOR_RED, ICON_ERROR, line_number, COLOR_RESET);
            error_count++;
            increment_error_count();
            continue;
        }
        
        new_student->first_name = strdup(first_name);
        new_student->last_name = strdup(last_name);
        new_student->date_of_birth = strdup(dob);
        new_student->status = strdup("pending");
        new_student->next = NULL;
        
        Save_student(db, list, new_student);
        
        // Success!
        imported_count++;
        increment_student_imported();
        
        // Show progress for large imports
        if (line_number % 10 == 0) {
            printf("\rProcessing line %d...", line_number);
            fflush(stdout);
        }
    }
    
    fclose(file);
    
    // Print summary
    printf("\n\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                      IMPORT SUMMARY                          ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║  Total lines processed:                                 %5d║\n", line_number);
    printf("║  Successfully imported:                                 %5d║\n", imported_count);
    printf("║  Errors encountered:                                    %5d║\n", error_count);
    printf("║  Success rate:                                         %5.1f%%║\n", 
           line_number > 0 ? (imported_count * 100.0) / line_number : 0.0);
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    
    if (imported_count > 0) {
        printf("%s%s Import completed successfully!%s\n", COLOR_GREEN, ICON_SUCCESS, COLOR_RESET);
    } else if (error_count > 0) {
        printf("%s%s Import failed - all lines had errors%s\n", COLOR_RED, ICON_ERROR, COLOR_RESET);
    } else {
        printf("%s%s No data found in file%s\n", COLOR_YELLOW, ICON_WARNING, COLOR_RESET);
    }
}

//Export to CSV function to be added later

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

    printf("Student with ID %d saved successfully.\n", student -> id);

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

void printStudentList(Student_List* list, Major_List* major_list, char* major_id){

    //First, check if major exists
    Major* major = major_list -> head;
    while(major != NULL && strcmp(major -> major_id, major_id) != 0){
        major = major -> next; 
    }   

    if(major == NULL){
        printf("Major with ID %s not found.\n", major_id);
        return; 
    }

    // Then, print students in that major
    Student* temp = list -> head; 

    printf("Number of students: %d\n\n", list -> num_of_students);
    printf("________________________________________________________________\n");
    printf("%-5s%-15s%-15s%-12s%-10s\n", "ID", "First Name", "Last Name", "DOB", "Status");
    printf("________________________________________________________________\n");
    while(temp != NULL && strcmp(temp -> major_id, major_id) == 0){
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