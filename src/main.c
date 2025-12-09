#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "sqlite3.h"
#include "database.h"

void MainMenu(){
    printf("\n__________________________________________\n");
    printf("Welcome to the Student Management System\n");
    printf("__________________________________________\n\n");

    printf("Which of the following are you?\n ");
    printf("_________________________________________\n");
    printf("1. Admin\n");
    printf("2. Student\n");
    printf("3. Exit\n");
    printf("_________________________________________\n");
    printf("Enter your choice (1, 2 or 3): ");

    return; 
}

void AdminFunctionMenu(){
    printf("\n_________________________________________\n");
    printf("Admin functionalities:\n");
    printf("_________________________________________\n");
    printf("1. Manage Students\n");
    printf("2. Manage Courses\n");
    printf("3. Manage Majors\n");
    printf("4. Exit\n");
    printf("_________________________________________\n"); 
    printf("Enter your choice (1, 2, 3 or 4): "); 

    return; 
}

void StudentManagementMenu(){
    printf("\n_________________________________________\n");
    printf("Student Management Menu:\n");
    printf("_________________________________________\n");
    printf("1. Add Student\n");
    printf("2. Remove Student\n");
    printf("3. Update Student\n");
    printf("4. View Student by ID\n");
    printf("5. View all Students\n");
    printf("6. Exit\n");
    printf("_________________________________________\n"); 
    printf("Enter your choice (1-6): "); 

    return; 
}   

void CourseManagementMenu(){
    printf("\n_________________________________________\n");
    printf("Course Management Menu:\n");
    printf("_________________________________________\n");
    printf("1. Add Course\n");
    printf("2. Remove Course\n");
    printf("3. Update Course\n");
    printf("4. Add Course to Major\n");
    printf("5. View Courses in a Major\n");
    printf("6. View All Courses\n");
    printf("7. Exit\n");
    printf("_________________________________________\n"); 
    printf("Enter your choice (1-7): "); 

    return; 
}


int main(){

    sqlite3 *test;
    char *err_msg = 0;
    db_connect(&test, "test.db");

    Student_List* StudentList = CreateList();
    load_students(test, StudentList);

    Course_List* CourseList = malloc(sizeof(Course_List));
    CourseList -> head = NULL;
    CourseList -> num_of_courses = 0;

    MainMenu();

    int choice; 
    scanf("%d", &choice);
    getchar();

    while(choice != 1 && choice != 2 && choice != 3){
        printf("Invalid choice. Please enter 1 for Admin, 2 for Student, or 3 to Exit: ");
        scanf("%d", &choice);
        getchar();
    }

    if(choice == 1){
        printf("Enter the password: ");
        char password[50]; 
        fgets(password, sizeof(password), stdin); 
        password[strcspn(password, "\n")] = '\0';

        if(strcmp(password, "admin123") != 0){
        printf("Error: Incorrect password\n");
        return 1; 
        }

        
        printf("_________________________________________\n");
        printf("Password accepted. Access granted.\nYou are logged in as Admin.\n");
        printf("_________________________________________\n\n");

        AdminFunctionMenu();

        int admin_choice;
        scanf("%d", &admin_choice);
        getchar();
        while(admin_choice < 1 || admin_choice > 4){
            printf("Invalid choice. Please enter 1, 2, 3 or 4: ");
            scanf("%d", &admin_choice);
            getchar();
        }

        if(admin_choice == 1){
            printf("Student management selected.\n");
            
            StudentManagementMenu();

            int choice;
            scanf("%d", &choice);
            getchar();
            while(choice < 1 || choice > 6){
                printf("Invalid choice. Please enter a number between 1 and 6: ");
                scanf("%d", &choice);
                getchar();
            }

            if(choice == 1){
                printf("Add Student selected.\n");
                printf("_________________________________________\n");
                Student* new_student = CreateNode();
                Save_student(test, StudentList, new_student);
                printf("_________________________________________\n");
                StudentManagementMenu();
            }

            else if(choice == 2){
                printf("Remove Student selected.\n");
                printf("_________________________________________\n");
                printf("Enter Student ID to remove: ");
                int id;
                scanf("%d", &id);
                getchar();
                removeStudent(test, StudentList, id);
                printf("_________________________________________\n");
                StudentManagementMenu();
            }

            else if(choice == 3){
                printf("Update Student selected.\n");
                printf("_________________________________________\n");
                printf("Enter Student ID to update: ");
                int id;
                scanf("%d", &id);
                getchar();
                updateStudent(test, StudentList, id);
                printf("_________________________________________\n");
                StudentManagementMenu();
            }

            else if(choice == 4){
                printf("View Student by ID selected.\n");
                printf("_________________________________________\n");
                printf("Enter Student ID to view: ");
                int id;
                scanf("%d", &id);
                getchar();
                printStudent(StudentList, id);
                printf("_________________________________________\n");
                StudentManagementMenu();
            }

            else if(choice == 5){
                printf("View All Students selected.\n");
                printf("_________________________________________\n");
                printList(StudentList);
                printf("_________________________________________\n");
                StudentManagementMenu();
            }

            else if(choice == 6){
                printf("Exiting Student Management.\n");
                _sleep(1000);
                AdminFunctionMenu();
            }
        }

        else if(admin_choice == 2){
            printf("Course management selected.\n");

            CourseManagementMenu();

            int course_choice;
            scanf("%d", &course_choice);
            getchar();
            while(course_choice < 1 || course_choice > 7){
                printf("Invalid choice. Please enter a number between 1 and 7: ");
                scanf("%d", &course_choice);
                getchar();
            }

            if(course_choice == 1){
                printf("Add Course selected.\n");
                printf("_________________________________________\n");
                Course* new_course = createCourse();
                SaveCourse(test, CourseList, new_course);
                printf("_________________________________________\n");
                CourseManagementMenu();
            }

            else if(course_choice == 2){
                printf("Remove Course selected.\n");
                printf("_________________________________________\n");
                printf("Enter Course ID to remove: ");
                char course_id[10];
                fgets(course_id, sizeof(course_id), stdin); 
                course_id[strcspn(course_id, "\n")] = '\0';
                removeCourse(test, CourseList, course_id);
                printf("_________________________________________\n");
                CourseManagementMenu();
            }

            else if(course_choice == 3){
                printf("Update Course selected.\n");
                printf("_________________________________________\n");
                printf("Enter Course ID to update: ");
                char course_id[10];
                fgets(course_id, sizeof(course_id), stdin);
                course_id[strcspn(course_id, "\n")] = '\0';
                updateCourse(test, CourseList, course_id);
                printf("_________________________________________\n");
                CourseManagementMenu();
            }

            else if(course_choice == 4){
                printf("Add Course to Major selected.\n");
                printf("_________________________________________\n");
                // To be implemented
            }

            else if(course_choice == 5){
                printf("View Courses in a Major selected.\n");
                printf("_________________________________________\n");
                // To be implemented
            }

            else if(course_choice == 6){
                printf("View All Courses selected.\n");
                printf("_________________________________________\n");
                printAllCourses(CourseList);
                printf("_________________________________________\n");
                CourseManagementMenu();

            }

            else if(course_choice == 7){
                printf("Exiting Course Management.\n");
                _sleep(1000);
                AdminFunctionMenu();
            }
        }

        else if(admin_choice == 3){
            printf("Major management selected.\n");
            // To be implemented
        }

    }


    
    









    sqlite3_close(test);
    return 0;
}    