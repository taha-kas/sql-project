#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "sqlite3.h"
#include "database.h"

Major* CreateMajor(){
    Major* new_major = malloc(sizeof(Major));

    if(new_major == NULL){
        printf("Error: Could not allocate memory for new major.\n");
        return NULL;
    }

    //Get Major ID:
    printf("Enter Major ID: ");
    fgets(new_major -> major_id, sizeof(new_major -> major_id), stdin); 
    new_major -> major_id[strcspn(new_major -> major_id, "\n")] = '\0';

    //Get Major Name:
    printf("Enter Major Name: ");
    fgets(new_major -> major_name, sizeof(new_major -> major_name), stdin); 
    new_major -> major_name[strcspn(new_major -> major_name, "\n")] = '\0';

    new_major -> courses = CreateCourseList(); 
    new_major -> num_of_courses = 0;
    new_major -> capacity = 30;
    new_major -> enrolled_students = 0;

    //Get Head Professor Name:
    printf("Enter Head Professor Name: ");
    fgets(new_major -> head_professor, sizeof(new_major -> head_professor), stdin);
    new_major -> head_professor[strcspn(new_major -> head_professor, "\n")] = '\0';
    
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
    return 1;
}

int SaveMajor(sqlite3 *db, Major_List* list, Major* major){

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