#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "sqlite3.h"
#include "database.h"
#include "ui_helpers.h"

void MainMenu(){
    printf("%s", CLEAR_SCREEN);
    print_banner("Student Management System", COLOR_MAGENTA);
    
    printf("\n%sSelect your role:%s\n\n", COLOR_WHITE, COLOR_RESET);
    printf("%s[1]%s Admin\n", COLOR_CYAN, COLOR_RESET);
    printf("%s[2]%s Student\n", COLOR_CYAN, COLOR_RESET);
    printf("%s[3]%s Exit\n", COLOR_CYAN, COLOR_RESET);
    
    printf("\n%s» Enter your choice (1-3):%s ", COLOR_YELLOW, COLOR_RESET);
}

void AdminFunctionMenu(){
    printf("%s", CLEAR_SCREEN);
    print_banner("Admin Dashboard", COLOR_BLUE);
    
    printf("\n%sAvailable Operations:%s\n\n", COLOR_WHITE, COLOR_RESET);
    printf("%s[1]%s Manage Students\n", COLOR_CYAN, COLOR_RESET);
    printf("%s[2]%s Manage Courses\n", COLOR_CYAN, COLOR_RESET);
    printf("%s[3]%s Manage Majors\n", COLOR_CYAN, COLOR_RESET);
    printf("%s[4]%s Return to Main Menu\n", COLOR_CYAN, COLOR_RESET);
    
    printf("\n%s» Select operation (1-4):%s ", COLOR_YELLOW, COLOR_RESET);
}

void StudentManagementMenu(){
    printf("%s", CLEAR_SCREEN);
    print_banner("Student Management", COLOR_GREEN);
    
    printf("\n%sAvailable Actions:%s\n\n", COLOR_WHITE, COLOR_RESET);
    printf("%s[1]%s Add New Student\n", COLOR_CYAN, COLOR_RESET);
    printf("%s[2]%s Import Students from CSV\n", COLOR_CYAN, COLOR_RESET);
    printf("%s[3]%s Remove Student\n", COLOR_CYAN, COLOR_RESET);
    printf("%s[4]%s Update Student Information\n", COLOR_CYAN, COLOR_RESET);
    printf("%s[5]%s View Student Details\n", COLOR_CYAN, COLOR_RESET);
    printf("%s[6]%s View All Students\n", COLOR_CYAN, COLOR_RESET);
    printf("%s[7]%s Return to Admin Menu\n", COLOR_CYAN, COLOR_RESET);
    
    printf("\n%s» Select action (1-7):%s ", COLOR_YELLOW, COLOR_RESET);
}

void CourseManagementMenu(){
    printf("%s", CLEAR_SCREEN);
    print_banner("Course Management", COLOR_BLUE);
    
    printf("\n%sAvailable Actions:%s\n\n", COLOR_WHITE, COLOR_RESET);
    printf("%s[1]%s Add New Course\n", COLOR_CYAN, COLOR_RESET);
    printf("%s[2]%s Remove Course\n", COLOR_CYAN, COLOR_RESET);
    printf("%s[3]%s Update Course\n", COLOR_CYAN, COLOR_RESET);
    printf("%s[4]%s Add Course to Major\n", COLOR_CYAN, COLOR_RESET);
    printf("%s[5]%s View All Courses\n", COLOR_CYAN, COLOR_RESET);
    printf("%s[6]%s Return to Admin Menu\n", COLOR_CYAN, COLOR_RESET);
    
    printf("\n%s» Select action (1-6):%s ", COLOR_YELLOW, COLOR_RESET);
}

void MajorManagementMenu(){
    printf("%s", CLEAR_SCREEN);
    print_banner("Major Management", COLOR_MAGENTA);
    
    printf("\n%sAvailable Actions:%s\n\n", COLOR_WHITE, COLOR_RESET);
    printf("%s[1]%s Add New Major\n", COLOR_CYAN, COLOR_RESET);
    printf("%s[2]%s Remove Major\n", COLOR_CYAN, COLOR_RESET);
    printf("%s[3]%s Update Major\n", COLOR_CYAN, COLOR_RESET);
    printf("%s[4]%s View All Majors\n", COLOR_CYAN, COLOR_RESET);
    printf("%s[5]%s View Courses in Major\n", COLOR_CYAN, COLOR_RESET);
    printf("%s[6]%s Return to Admin Menu\n", COLOR_CYAN, COLOR_RESET);
    
    printf("\n%s» Select action (1-6):%s ", COLOR_YELLOW, COLOR_RESET);
}

//fonction pour activer le support unicode dans la console windows
void enable_unicode_support(void) {
    // Set console to UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    // Also try changing font
    CONSOLE_FONT_INFOEX font = {0};
    font.cbSize = sizeof(font);
    font.dwFontSize.Y = 14;
    wcscpy(font.FaceName, L"Consolas");  // Or "Lucida Console"
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &font);
}

int main() {

    // Enable Unicode support
    enable_unicode_support();

    // Initialize session tracking
    initialize_session();
    
    // Print welcome banner
    print_welcome_banner();
    
    // Database setup
    sqlite3 *test;
    char *err_msg = 0;

    //Delete all existing data in the database for testing purposes
    sqlite3_exec(test, "DELETE FROM students;", NULL, NULL, &err_msg);
    sqlite3_exec(test, "DELETE FROM majors;", NULL, NULL, &err_msg);
    sqlite3_exec(test, "DELETE FROM courses;", NULL, NULL, &err_msg);
    
    print_info("Initializing database...");
    Sleep(500);
    db_connect(&test, "test.db");
    
    // Load data structures
    print_info("Loading student records...");
    Sleep(500);
    Student_List* StudentList = CreateList();
    load_students(test, StudentList);
    
    //a ajouter apres avoir rempli la table major et la table course
    print_info("Loading major records...");
    Sleep(500);
    Major_List* MajorList = CreateMajorList();
    
    print_info("Loading course records...");
    Sleep(500);
    Course_List* CourseList = malloc(sizeof(Course_List));
    CourseList->head = NULL;
    CourseList->num_of_courses = 0;
    
    print_success("System initialization complete!");
    printf("\n%sPress Enter to continue...%s", COLOR_YELLOW, COLOR_RESET);
    getchar();
    
    int exit_program = 0;
    
    while (!exit_program){
        MainMenu();
        
        int choice = get_numeric_input("", 1, 3);
        if (choice == -1) continue;
        
        increment_command_count();
        
        if(choice == 1){
            printf("\n%s» Enter admin password:%s ", COLOR_YELLOW, COLOR_RESET);
            
            char password[50];
            fgets(password, sizeof(password), stdin);
            password[strcspn(password, "\n")] = '\0';
            
            if(strcmp(password, "admin123") != 0){
                print_error("Access denied: Incorrect password");
                Sleep(1500);
                continue;
            }
            
            print_success("Access granted. Welcome, Admin!");
            Sleep(1000);
            
            int admin_exit = 0;
            
            while (!admin_exit) {
                AdminFunctionMenu();
                
                int admin_choice = get_numeric_input("", 1, 4);
                if (admin_choice == -1) continue;
                
                increment_command_count();
                
                if(admin_choice == 1){
                    int student_management_exit = 0;
                    
                    while (!student_management_exit) {
                        StudentManagementMenu();
                        
                        int student_choice = get_numeric_input("", 1, 7);
                        if (student_choice == -1) continue;
                        
                        increment_command_count();
                        
                        if(student_choice == 1){
                            print_banner("Add New Student", COLOR_GREEN);
                            printf("\n");
                            
                            Student* new_student = CreateNode();
                            Save_student(test, StudentList, new_student);
                            
                            increment_student_added();
                            print_success("Student added successfully!");
                            
                            printf("\n%sPress Enter to continue...%s", COLOR_YELLOW, COLOR_RESET);
                            getchar();
                        }
                        else if(student_choice == 2){
                            print_banner("Import Students from CSV", COLOR_GREEN);
                            printf("\n");
                            
                            char filename[100];
                            printf("%s» Enter CSV filename:%s ", COLOR_YELLOW, COLOR_RESET);
                            fgets(filename, sizeof(filename), stdin);
                            filename[strcspn(filename, "\n")] = '\0';
                            
                            print_info("Importing students...");
                            
                            // Simulate progress bar for CSV import
                            printf("\n");
                            for (int i = 0; i <= 100; i += 5) {
                                display_progress_bar(i, 100, 50);
                                Sleep(50); // Simulate processing time
                            }
                            printf("\n");
                            
                            importFromCSV(test, StudentList, filename);
                            
                            print_success("CSV import completed!");
                            
                            printf("\n%sPress Enter to continue...%s", COLOR_YELLOW, COLOR_RESET);
                            getchar();
                        }
                        else if(student_choice == 3){
                            print_banner("Remove Student", COLOR_RED);
                            printf("\n");
                            
                            int id;
                            printf("%s» Enter Student ID to remove:%s ", COLOR_YELLOW, COLOR_RESET);
                            scanf("%d", &id);
                            clear_input_buffer();
                            
                            print_warning("Removing student...");
                            Sleep(1000);
                            
                            removeStudent(test, StudentList, id);
                            print_success("Student removed successfully!");
                            
                            printf("\n%sPress Enter to continue...%s", COLOR_YELLOW, COLOR_RESET);
                            getchar();
                        }
                        else if(student_choice == 4){
                            print_banner("Update Student", COLOR_YELLOW);
                            printf("\n");
                            
                            int id;
                            printf("%s» Enter Student ID to update:%s ", COLOR_YELLOW, COLOR_RESET);
                            scanf("%d", &id);
                            clear_input_buffer();
                            
                            print_info("Updating student information...");
                            Sleep(1000);
                            
                            updateStudent(test, StudentList, id);
                            print_success("Student updated successfully!");
                            
                            printf("\n%sPress Enter to continue...%s", COLOR_YELLOW, COLOR_RESET);
                            getchar();
                        }
                        else if(student_choice == 5){
                            print_banner("View Student Details", COLOR_CYAN);
                            printf("\n");
                            
                            int id;
                            printf("%s» Enter Student ID to view:%s ", COLOR_YELLOW, COLOR_RESET);
                            scanf("%d", &id);
                            clear_input_buffer();
                            
                            print_info("Fetching student details...");
                            Sleep(500);
                            
                            printStudent(StudentList, id);
                            
                            printf("\n%sPress Enter to continue...%s", COLOR_YELLOW, COLOR_RESET);
                            getchar();
                        }
                        else if(student_choice == 6){
                            print_banner("All Students", COLOR_CYAN);
                            printf("\n");
                            
                            print_info("Loading student list...");
                            Sleep(500);
                            
                            // printStudentList(StudentList, MajorList, NULL);
                            
                            printf("\n%sPress Enter to continue...%s", COLOR_YELLOW, COLOR_RESET);
                            getchar();
                        }
                        else if(student_choice == 7){
                            print_info("Returning to Admin Menu...");
                            Sleep(1000);
                            student_management_exit = 1;
                        }
                    }
                }
                else if(admin_choice == 2){
                    int course_management_exit = 0;
                    
                    while (!course_management_exit) {
                        CourseManagementMenu();
                        
                        int course_choice = get_numeric_input("", 1, 6);
                        if (course_choice == -1) continue;
                        
                        increment_command_count();
                        
                        if(course_choice == 1){
                            print_banner("Add New Course", COLOR_GREEN);
                            printf("\n");
                            
                            Course* new_course = createCourse();
                            
                            print_info("Adding course...");
                            Sleep(1000);
                            
                            SaveCourse(test, CourseList, new_course);
                            increment_course_added();
                            print_success("Course added successfully!");
                            
                            printf("\n%sPress Enter to continue...%s", COLOR_YELLOW, COLOR_RESET);
                            getchar();
                        }
                        else if(course_choice == 2){
                            print_banner("Remove Course", COLOR_RED);
                            printf("\n");
                            
                            char course_id[10];
                            printf("%s» Enter Course ID to remove:%s ", COLOR_YELLOW, COLOR_RESET);
                            fgets(course_id, sizeof(course_id), stdin);
                            course_id[strcspn(course_id, "\n")] = '\0';
                            
                            print_warning("Removing course...");
                            Sleep(1000);
                            
                            removeCourse(test, CourseList, course_id);
                            print_success("Course removed successfully!");
                            
                            printf("\n%sPress Enter to continue...%s", COLOR_YELLOW, COLOR_RESET);
                            getchar();
                        }
                        else if(course_choice == 3){
                            print_banner("Update Course", COLOR_YELLOW);
                            printf("\n");
                            
                            char course_id[10];
                            printf("%s» Enter Course ID to update:%s ", COLOR_YELLOW, COLOR_RESET);
                            fgets(course_id, sizeof(course_id), stdin);
                            course_id[strcspn(course_id, "\n")] = '\0';
                            
                            print_info("Updating course...");
                            Sleep(1000);
                            
                            updateCourse(test, CourseList, course_id);
                            print_success("Course updated successfully!");
                            
                            printf("\n%sPress Enter to continue...%s", COLOR_YELLOW, COLOR_RESET);
                            getchar();
                        }
                        else if(course_choice == 4){
                            print_banner("Add Course to Major", COLOR_BLUE);
                            printf("\n");
                            
                            char major_id[10];
                            printf("%s» Enter Major ID:%s ", COLOR_YELLOW, COLOR_RESET);
                            fgets(major_id, sizeof(major_id), stdin);
                            major_id[strcspn(major_id, "\n")] = '\0';
                            
                            char course_id[10];
                            printf("%s» Enter Course ID:%s ", COLOR_YELLOW, COLOR_RESET);
                            fgets(course_id, sizeof(course_id), stdin);
                            course_id[strcspn(course_id, "\n")] = '\0';
                            
                            char course_name[100];
                            printf("%s» Enter Course Name:%s ", COLOR_YELLOW, COLOR_RESET);
                            fgets(course_name, sizeof(course_name), stdin);
                            course_name[strcspn(course_name, "\n")] = '\0';
                            
                            char professor[100];
                            printf("%s» Enter Professor Name:%s ", COLOR_YELLOW, COLOR_RESET);
                            fgets(professor, sizeof(professor), stdin);
                            professor[strcspn(professor, "\n")] = '\0';
                            
                            print_info("Adding course to major...");
                            Sleep(1000);
                            
                            add_course_to_major(test, MajorList, major_id, course_id, course_name, professor);
                            print_success("Course added to major successfully!");
                            
                            printf("\n%sPress Enter to continue...%s", COLOR_YELLOW, COLOR_RESET);
                            getchar();
                        }
                        else if(course_choice == 5){
                            print_banner("All Courses", COLOR_CYAN);
                            printf("\n");
                            
                            print_info("Loading course list...");
                            Sleep(500);
                            
                            printAllCourses(CourseList);
                            
                            printf("\n%sPress Enter to continue...%s", COLOR_YELLOW, COLOR_RESET);
                            getchar();
                        }
                        else if(course_choice == 6){
                            print_info("Returning to Admin Menu...");
                            Sleep(1000);
                            course_management_exit = 1;
                        }
                    }
                }
                else if(admin_choice == 3){
                    int major_management_exit = 0;
                    
                    while (!major_management_exit) {
                        MajorManagementMenu();
                        
                        int major_choice = get_numeric_input("", 1, 6);
                        if (major_choice == -1) continue;
                        
                        increment_command_count();
                        
                        if(major_choice == 1){
                            print_banner("Add New Major", COLOR_GREEN);
                            printf("\n");
                            
                            Major* new_major = CreateMajor();
                            
                            print_info("Adding major...");
                            Sleep(1000);
                            
                            Insert_major_list(MajorList, new_major);
                            db_insert_major(test, new_major);
                            increment_major_added();
                            print_success("Major added successfully!");
                            
                            printf("\n%sPress Enter to continue...%s", COLOR_YELLOW, COLOR_RESET);
                            getchar();
                        }
                        else if(major_choice == 2){
                            print_banner("Remove Major", COLOR_RED);
                            printf("\n");
                            
                            char major_id[10];
                            printf("%s» Enter Major ID to remove:%s ", COLOR_YELLOW, COLOR_RESET);
                            fgets(major_id, sizeof(major_id), stdin);
                            major_id[strcspn(major_id, "\n")] = '\0';
                            
                            print_warning("Removing major...");
                            Sleep(1000);
                            
                            removeMajor(test, MajorList, major_id);
                            print_success("Major removed successfully!");
                            
                            printf("\n%sPress Enter to continue...%s", COLOR_YELLOW, COLOR_RESET);
                            getchar();
                        }
                        else if(major_choice == 3){
                            print_banner("Update Major", COLOR_YELLOW);
                            printf("\n");
                            
                            char major_id[10];
                            printf("%s» Enter Major ID to update:%s ", COLOR_YELLOW, COLOR_RESET);
                            fgets(major_id, sizeof(major_id), stdin);
                            major_id[strcspn(major_id, "\n")] = '\0';
                            
                            print_info("Updating major...");
                            Sleep(1000);
                            
                            UpdateMajor(test, MajorList, major_id);
                            print_success("Major updated successfully!");
                            
                            printf("\n%sPress Enter to continue...%s", COLOR_YELLOW, COLOR_RESET);
                            getchar();
                        }
                        else if(major_choice == 4){
                            print_banner("All Majors", COLOR_CYAN);
                            printf("\n");
                            
                            print_info("Loading major list...");
                            Sleep(500);
                            
                            printAllMajors(MajorList);
                            
                            printf("\n%sPress Enter to continue...%s", COLOR_YELLOW, COLOR_RESET);
                            getchar();
                        }
                        else if(major_choice == 5){
                            print_banner("Courses in Major", COLOR_BLUE);
                            printf("\n");
                            
                            char major_id[10];
                            printf("%s» Enter Major ID to view courses:%s ", COLOR_YELLOW, COLOR_RESET);
                            fgets(major_id, sizeof(major_id), stdin);
                            major_id[strcspn(major_id, "\n")] = '\0';
                            
                            print_info("Fetching courses...");
                            Sleep(500);
                            
                            print_courses_in_major(MajorList, major_id);
                            
                            printf("\n%sPress Enter to continue...%s", COLOR_YELLOW, COLOR_RESET);
                            getchar();
                        }
                        else if(major_choice == 6){
                            print_info("Returning to Admin Menu...");
                            Sleep(1000);
                            major_management_exit = 1;
                        }
                    }
                }
                else if(admin_choice == 4){
                    print_info("Returning to Main Menu...");
                    Sleep(1000);
                    admin_exit = 1;
                }
            }
        }
        else if(choice == 2){
            print_banner("Student Portal", COLOR_GREEN);
            printf("\n%sStudent functionalities are coming soon!%s\n", COLOR_YELLOW, COLOR_RESET);
            printf("%sPress Enter to return to main menu...%s", COLOR_YELLOW, COLOR_RESET);
            getchar();
        }
        else if(choice == 3){
            printf("\n%sExiting program...%s\n", COLOR_YELLOW, COLOR_RESET);
            Sleep(1000);
            exit_program = 1;
        }
    }
    
    // Cleanup and show session summary
    print_info("Closing database connection...");
    sqlite3_close(test);
    
    print_session_summary();
    
    printf("\n%sPress Enter to exit...%s", COLOR_YELLOW, COLOR_RESET);
    getchar();
    
    return 0;
}