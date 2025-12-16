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
    printf("3. Exit\n"); //if this is selected, program ends (return 0 in main)
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
    printf("2. Import Students from CSV\n");
    printf("3. Remove Student\n");
    printf("4. Update Student\n");
    printf("5. View Student by ID\n");
    printf("6. View all Students\n");
    printf("7. Exit\n");
    printf("_________________________________________\n"); 
    printf("Enter your choice (1-7): "); 

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
    printf("5. View All Courses\n");
    printf("6. Exit\n");
    printf("_________________________________________\n"); 
    printf("Enter your choice (1-6): "); 

    return; 
}

void MajorManagementMenu(){
    printf("\n_________________________________________\n");
    printf("Major Management Menu:\n");
    printf("_________________________________________\n");
    printf("1. Add Major\n");
    printf("2. Remove Major\n");
    printf("3. Update Major\n");
    printf("4. View All Majors\n");
    printf("5. View Courses in Major\n");
    printf("6. Exit\n");
    printf("_________________________________________\n"); 
    printf("Enter your choice (1-5): "); 

    return; 
}


int main() {
    sqlite3 *test;
    char *err_msg = 0;
    db_connect(&test, "test.db");

    Student_List* StudentList = CreateList();
    load_students(test, StudentList); // Use this function to load existing students from the database if needed

    Major_List* MajorList = CreateMajorList();
    Course_List* CourseList = malloc(sizeof(Course_List));
    CourseList -> head = NULL;
    CourseList -> num_of_courses = 0;

    // CreateCourseTable(test);
    // CreateMajorTable(test);

    

    int exit_program = 0;
    
    while (!exit_program){
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
                continue; // Go back to main menu
            }
            
            printf("_________________________________________\n");
            printf("Password accepted. Access granted.\nYou are logged in as Admin.\n");
            printf("_________________________________________\n\n");

            int admin_exit = 0;
            
            while (!admin_exit) {
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
                    
                    int student_management_exit = 0;
                    
                    while (!student_management_exit) {
                        StudentManagementMenu();

                        int student_choice;
                        scanf("%d", &student_choice);
                        getchar();
                        
                        while(student_choice < 1 || student_choice > 7){
                            printf("Invalid choice. Please enter a number between 1 and 7: ");
                            scanf("%d", &student_choice);
                            getchar();
                        }

                        if(student_choice == 1){
                            printf("Add Student selected.\n");
                            printf("_________________________________________\n");

                            Student* new_student = CreateNode();
                            Save_student(test, StudentList, new_student);
                            
                            printf("_________________________________________\n");
                            // Will loop back to student menu
                        }
                        else if(student_choice == 2){
                            printf("Import Students from CSV selected.\n");
                            printf("_________________________________________\n");

                            char filename[100];
                            printf("Enter CSV filename: ");
                            fgets(filename, sizeof(filename), stdin);
                            filename[strcspn(filename, "\n")] = '\0';

                            printf("Importing students from %s...\n", filename);
                            Sleep(2000);

                            importFromCSV(test, StudentList, filename);
                            printf("_________________________________________\n");
                        }
                        else if(student_choice == 3){
                            printf("Remove Student selected.\n");
                            printf("_________________________________________\n");
                            printf("Enter Student ID to remove: ");
                            int id;
                            scanf("%d", &id);
                            getchar();

                            printf("Removing Student ID %d...\n", id);
                            Sleep(2000);

                            removeStudent(test, StudentList, id);
                            printf("_________________________________________\n");
                        }
                        else if(student_choice == 4){
                            printf("Update Student selected.\n");
                            printf("_________________________________________\n");
                            printf("Enter Student ID to update: ");
                            int id;
                            scanf("%d", &id);
                            getchar();

                            printf("Updating Student ID %d...\n", id);
                            Sleep(2000);

                            updateStudent(test, StudentList, id);
                            printf("_________________________________________\n");
                        }
                        else if(student_choice == 5){
                            printf("View Student by ID selected.\n");
                            printf("_________________________________________\n");

                            printf("Enter Student ID to view: ");
                            int id;
                            scanf("%d", &id);
                            getchar();

                            printf("Fetching details for Student ID %d...\n", id);
                            Sleep(2000);

                            printStudent(StudentList, id);
                            printf("_________________________________________\n");
                        }
                        else if(student_choice == 6){
                            printf("View All Students selected.\n");
                            printf("_________________________________________\n");

                            printf("Printing all students...\n");
                            Sleep(2000);

                            printList(StudentList);
                            printf("_________________________________________\n");
                        }
                        else if(student_choice == 7){
                            printf("Exiting Student Management...\n");
                            Sleep(1000);
                            student_management_exit = 1; // Exit student management
                        }
                    }
                }
                else if(admin_choice == 2){
                    printf("Course management selected.\n");

                    int course_management_exit = 0;
                    
                    while (!course_management_exit) {
                        CourseManagementMenu();

                        int course_choice;
                        scanf("%d", &course_choice);
                        getchar();
                        
                        while(course_choice < 1 || course_choice > 6){
                            printf("Invalid choice. Please enter a number between 1 and 6: ");
                            scanf("%d", &course_choice);
                            getchar();
                        }

                        if(course_choice == 1){
                            printf("Add Course selected.\n");
                            printf("_________________________________________\n");

                            Course* new_course = createCourse();

                            printf("Adding Course %s...\n", new_course->course_id);
                            Sleep(2000);

                            SaveCourse(test, CourseList, new_course);
                            printf("_________________________________________\n");
                        }
                        else if(course_choice == 2){
                            printf("Remove Course selected.\n");
                            printf("_________________________________________\n");

                            printf("Enter Course ID to remove: ");
                            char course_id[10];
                            fgets(course_id, sizeof(course_id), stdin); 
                            course_id[strcspn(course_id, "\n")] = '\0';

                            printf("Removing Course %s...\n", course_id);
                            Sleep(2000);

                            removeCourse(test, CourseList, course_id);
                            printf("_________________________________________\n");
                        }
                        else if(course_choice == 3){
                            printf("Update Course selected.\n");
                            printf("_________________________________________\n");

                            printf("Enter Course ID to update: ");
                            char course_id[10];
                            fgets(course_id, sizeof(course_id), stdin);
                            course_id[strcspn(course_id, "\n")] = '\0';

                            printf("Updating Course %s...\n", course_id);
                            Sleep(2000);

                            updateCourse(test, CourseList, course_id);
                            printf("_________________________________________\n");
                        }
                        else if(course_choice == 4){
                            printf("Add Course to Major selected.\n");
                            printf("_________________________________________\n");

                            char major_id[10];
                            printf("Enter Major ID: ");
                            fgets(major_id, sizeof(major_id), stdin);
                            major_id[strcspn(major_id, "\n")] = '\0'; 

                            char course_id[10];
                            printf("Enter Course ID: ");
                            fgets(course_id, sizeof(course_id), stdin);
                            course_id[strcspn(course_id, "\n")] = '\0';

                            char course_name[100];
                            printf("Enter Course Name: ");
                            fgets(course_name, sizeof(course_name), stdin);
                            course_name[strcspn(course_name, "\n")] = '\0';

                            char professor[100];
                            printf("Enter Professor Name: ");
                            fgets(professor, sizeof(professor), stdin);
                            professor[strcspn(professor, "\n")] = '\0';

                            printf("Adding Course %s to Major %s...\n", course_name, major_id);
                            Sleep(2000);

                            add_course_to_major(test, MajorList, major_id, course_id, course_name, professor);
                            printf("_________________________________________\n");
                        }
                        else if(course_choice == 5){
                            printf("View All Courses selected.\n");
                            printf("printing all courses...\n");
                            Sleep(2000);
                            printf("_________________________________________\n");

                            printAllCourses(CourseList);
                            printf("_________________________________________\n");
                        }
                        else if(course_choice == 6){
                            printf("Exiting Course Management...\n");
                            Sleep(2000);
                            course_management_exit = 1; // Exit course management
                        }
                    }
                }
                else if(admin_choice == 3){
                    printf("Major management selected.\n");

                    int major_management_exit = 0;
                    
                    while (!major_management_exit) {
                        MajorManagementMenu();

                        int major_choice;
                        scanf("%d", &major_choice);
                        getchar();

                        while(major_choice < 1 || major_choice > 6){
                            printf("Invalid choice. Please enter a number between 1 and 6: ");
                            scanf("%d", &major_choice);
                            getchar();
                        }

                        if(major_choice == 1){
                            printf("Add Major selected.\n");
                            printf("_________________________________________\n");

                            Major* new_major = CreateMajor();

                            printf("Adding Major %s...\n", new_major->major_id);
                            Sleep(2000);

                            Insert_major_list(MajorList, new_major);
                            db_insert_major(test, new_major);
                            printf("_________________________________________\n");
                        }
                        else if(major_choice == 2){
                            printf("Remove Major selected.\n");
                            printf("_________________________________________\n");

                            char major_id[10];
                            printf("Enter Major ID to remove: ");
                            fgets(major_id, sizeof(major_id), stdin);
                            major_id[strcspn(major_id, "\n")] = '\0';

                            printf("Removing Major %s...\n", major_id);
                            Sleep(2000);

                            removeMajor(test, MajorList, major_id);
                            printf("_________________________________________\n");
                        }
                        else if(major_choice == 3){
                            printf("Update Major selected.\n");
                            printf("_________________________________________\n");

                            char major_id[10];
                            printf("Enter Major ID to update: ");
                            fgets(major_id, sizeof(major_id), stdin);
                            major_id[strcspn(major_id, "\n")] = '\0';

                            printf("Updating Major %s...\n", major_id);
                            Sleep(2000);

                            UpdateMajor(test, MajorList, major_id);
                            printf("_________________________________________\n");
                        }
                        else if(major_choice == 4){
                            printf("View All Majors selected.\n");
                            printf("Printing all majors...\n");
                            Sleep(2000);
                            printf("_________________________________________\n");

                            printAllMajors(MajorList);
                            printf("_________________________________________\n");
                        }
                        else if(major_choice == 5){
                            printf("View Courses in Major selected.\n");
                            printf("_________________________________________\n");

                            char major_id[10];
                            printf("Enter Major ID to view courses: ");
                            fgets(major_id, sizeof(major_id), stdin);
                            major_id[strcspn(major_id, "\n")] = '\0';

                            printf("Fetching courses for Major %s...\n", major_id);
                            Sleep(2000);

                            print_courses_in_major(MajorList, major_id);
                            printf("_________________________________________\n");
                        }
                        else if(major_choice == 6){
                            printf("Exiting Major Management...\n");
                            Sleep(2000);
                            major_management_exit = 1; // Exit major management
                        }
                    }
                }
                else if(admin_choice == 4){
                    printf("Exiting Admin Menu...\n");
                    Sleep(1000);
                    admin_exit = 1; // Exit admin menu
                }
            }
        }
        else if(choice == 2){
            printf("Student functionalities are not yet implemented.\n");
            printf("Press Enter to continue...");
            getchar();
        }
        else if(choice == 3){
            printf("Exiting program. Goodbye!\n");
            Sleep(2000);
            exit_program = 1; // Exit program
        }
    }
    
    sqlite3_close(test);
    return 0;
}