#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "sqlite3.h"
#include "database.h"

Major_List* CreateMajorList(){
    Major_List* temp = malloc(sizeof(Major_List)); 

    if(temp == NULL){
        printf("Error: Could not allocate memory\n"); 
        return NULL; 
    }

    temp -> head = NULL; 
    temp -> num_of_majors = 0; 

    return temp; 
}

Major* CreateMajor(){
    Major* new_major = malloc(sizeof(Major));

    if(new_major == NULL){
        printf("Error: Could not allocate memory for new major.\n");
        return NULL;
    }

    //Get Major ID:
    printf("Enter Major ID: ");
    char major_id[50];
    fgets(major_id, sizeof(major_id), stdin); 
    major_id[strcspn(major_id, "\n")] = '\0';
    new_major -> major_id = strdup(major_id);

    //Get Major Name:
    printf("Enter Major Name: ");
    char major_name[100];
    fgets(major_name, sizeof(major_name), stdin); 
    major_name[strcspn(major_name, "\n")] = '\0';
    new_major -> major_name = strdup(major_name);
    
    //Get Head Professor Name:
    printf("Enter Head Professor Name: ");
    char head_professor_name[50];
    fgets(head_professor_name, sizeof(head_professor_name), stdin);
    head_professor_name[strcspn(head_professor_name, "\n")] = '\0';
    new_major -> head_professor = strdup(head_professor_name);  
    
    new_major -> capacity = 30;
    new_major -> course_list = CreateCourseList();
    new_major -> courses_loaded = 0; 
    new_major -> enrolled_students = 0;
    new_major -> next = NULL;

    return new_major;
}

int Insert_major_list(Major_List* list, Major* major) {

    if(list == NULL || major == NULL) {
        printf("Error: List or major is NULL\n");
        return 0; 
    }

    else if(list -> head == NULL){
        list -> head = major;
        list -> num_of_majors++;
        return 1; 
    }

    Major* temp = list -> head; 
    while(temp -> next != NULL){
        temp = temp -> next; 
    }
    temp -> next = major;
    list -> num_of_majors++;
    major -> courses_loaded = 1;
    return 1;
}

int SaveMajor(sqlite3* db, Major_List* list, Major* major){

    if(!db_insert_major(db, major)){
        printf("Error: Could not insert major into database\n");
        return 0;
    }

    if(!Insert_major_list(list, major)){
        printf("Error: Could not insert major into list\n");
        db_remove_major(db, major->major_id);
        return 0;
    }

    return 1;
}

int removeMajor(sqlite3* db, Major_List* list, char* major_id){
    Major* temp = list -> head; 
    Major* prev = NULL; 

    while(temp != NULL && strcmp(major_id, temp -> major_id) != 0){
        prev = temp; 
        temp = temp -> next;
    }

    if(temp == NULL){
        printf("Major with ID %s not found.\n", major_id);
        return 0; 
    }

    if(prev == NULL){
        list -> head = temp -> next;
    }
    
    else{
        prev -> next = temp -> next; 
    }

    if(!db_remove_major(db, temp -> major_id)){
        printf("Error: Could not remove major from database\n");
        return 0; 
    }
    
    free(temp -> major_id);
    free(temp -> major_name);
    free(temp -> head_professor);
    freeCourseList(temp -> course_list);
    free(temp);

    list -> num_of_majors--;
    printf("Major with ID %s has been removed successfully.\n", major_id);

    return 1;
}

//We could also consider adding the option of updating the major id too...
int UpdateMajor(sqlite3 *db, Major_List* list, char* major_id){
    Major* temp = list -> head; 

    while(temp != NULL && strcmp(temp -> major_id, major_id) != 0){
        temp = temp -> next; 
    }

    if(temp == NULL){
        printf("Major with ID %s not found.\n", major_id);
        return 0; 
    }

    printf("===============================================================\n");
    printf("Current details:\n");
    printf("===============================================================\n");
    printf("Major ID: %s\n", temp -> major_id);
    printf("===============================================================\n");
    printf("Major Name: %s\n", temp -> major_name);
    printf("===============================================================\n");
    printf("Head Professor: %s\n", temp -> head_professor);
    printf("===============================================================\n");
    printf("Capacity: %d\n", temp -> capacity);
    printf("===============================================================\n");
    printf("Enrolled Students: %d\n", temp -> enrolled_students);
    printf("===============================================================\n");
    printf("\n\nWhat fields do you want to update?\n");
    printf("[1]Major ID\n[2] Major Name\n[3] Head Professor\n[4] Capacity\n");
    printf("Enter your choice: ");
    char choice[10];
    fgets(choice, sizeof(choice), stdin);
    choice[strcspn(choice, "\n")] = 0;
    
    if(strcmp(choice, "1") == 0){
        char new_major_id[50];
        printf("Enter new Major ID: ");
        fgets(new_major_id, sizeof(new_major_id), stdin);
        new_major_id[strcspn(new_major_id, "\n")] = 0;
        temp -> major_id = strdup(new_major_id);
    }
    else if(strcmp(choice, "2") == 0){
        char new_major_name[100];
        printf("Enter new Major Name: ");
        fgets(new_major_name, sizeof(new_major_name), stdin);
        new_major_name[strcspn(new_major_name, "\n")] = 0;
        temp -> major_name = strdup(new_major_name);
    }
    else if(strcmp(choice, "3") == 0){
        char new_head_professor[50];
        printf("Enter new Head Professor Name: ");
        fgets(new_head_professor, sizeof(new_head_professor), stdin);
        new_head_professor[strcspn(new_head_professor, "\n")] = 0;
        temp -> head_professor = strdup(new_head_professor);
    }
    else if(strcmp(choice, "4") == 0){
        int new_capacity;
        printf("Enter new Capacity: ");
        scanf("%d", &new_capacity);
        getchar();
        temp -> capacity = new_capacity;
    }
    else{
        printf("Invalid choice.\n");
        return 0; 
    }

    printf("Major with ID %s updated successfully.\n", major_id);

    if(!db_update_major(db, temp)){
        printf("Error: Could not update major in database\n");
        return 0; 
    }

    return 1;
}

int printMajor(Major_List* list, char* major_id){
    Major* temp = list -> head; 

    while(temp != NULL && strcmp(temp -> major_id, major_id) != 0){
        temp = temp -> next; 
    }

    if(temp == NULL){
        printf("Major with ID %s not found.\n", major_id);
        return 0; 
    }

    printf("________________________________________________________________\n");
    printf("Major %s:\n", temp -> major_name);
    printf("________________________________________________________________\n");
    printf("%-15s%-30s%-20s\n", "Major ID", "Major Name", "Head Professor");
    printf("________________________________________________________________\n");
    printf("%-15s%-30s%-20s\n", temp -> major_id, temp -> major_name, temp ->head_professor);
    printf("________________________________________________________________\n");

    return 1; 
}

int printAllMajors(Major_List* list){
    Major* temp = list -> head; 

    if(temp == NULL || list -> num_of_majors == 0){
        printf("No majors available.\n");
        return 0; 
    }
    
    printf("___________________________________________________________________________________________________\n");
    printf("List of Majors:\n");
    printf("___________________________________________________________________________________________________\n");
    printf("%-15s%-65s%-30s\n", "Major ID", "Major Name", "Head Professor");
    printf("___________________________________________________________________________________________________\n");
    while(temp != NULL){
        printf("%-15s%-65s%-30s\n", temp -> major_id, temp -> major_name, temp ->head_professor);
        temp = temp -> next;
    }
    printf("___________________________________________________________________________________________________\n");

    return 1;
}
