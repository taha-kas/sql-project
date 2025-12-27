#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "sqlite3.h"
#include "database.h"
#include "ui_helpers.h"
#include "enrollment.h"
#include "schema.h"
#include "condition.h"
#include "grade.h"
#include "course_list.h"


////////////////////////////////////////ADMIN SECTION MENUS/////////////////////////////////////////
void MainMenu(){
    printf("%s", CLEAR_SCREEN);
    print_banner("Student Management System", COLOR_MAGENTA);
    
    printf("\nSelect your role:\n\n");
    print_info("[1] Admin\n");
    print_info("[2] Student\n");
    print_info("[3] Exit\n");
    
    print_warning("» Enter your choice (1-3): ");
}

void AdminFunctionMenu(){
    printf("%s", CLEAR_SCREEN);
    print_banner("Admin Dashboard", COLOR_BLUE);
    
    printf("\nAvailable Operations:\n\n");
    print_info("[1] Manage Students\n");
    print_info("[2] Manage Courses\n");
    print_info("[3] Manage Majors\n");
    print_info("[4] Return to Main Menu\n");
    
    print_warning("» Select operation (1-4): ");
}

void StudentManagementMenu(){
    printf("%s", CLEAR_SCREEN);
    print_banner("Student Management", COLOR_GREEN);
    
    printf("\nAvailable Actions:\n\n");
    print_info("[1] Add New Student\n");
    print_info("[2] Import Students from CSV\n");
    print_info("[3] Export Students to CSV\n");
    print_info("[4] Remove Student\n");
    print_info("[5] Update Student Information\n");
    print_info("[6] View Student Details\n");
    print_info("[7] View All Students\n");
    print_info("[8] Return to Admin Menu\n");
    
    print_warning("» Select action (1-8): ");
}

void CourseManagementMenu(){
    printf("%s", CLEAR_SCREEN);
    print_banner("Course Management", COLOR_BLUE);
    
    printf("\nAvailable Actions:\n\n");
    print_info("[1] Add New Course\n");
    print_info("[2] Remove Course\n");
    print_info("[3] Update Course\n");
    print_info("[4] Add Course to Major\n");
    print_info("[5] View All Courses\n");
    print_info("[6] Return to Admin Menu\n");
    
    print_warning("» Select action (1-6): ");
}

void MajorManagementMenu(){
    printf("%s", CLEAR_SCREEN);
    print_banner("Major Management", COLOR_MAGENTA);
    
    printf("\nAvailable Actions:\n\n");
    print_info("[1] Add New Major\n");
    print_info("[2] Remove Major\n");
    print_info("[3] Update Major\n");
    print_info("[4] View All Majors\n");
    print_info("[5] View Courses in Major\n");
    print_info("[6] Return to Admin Menu\n");
    
    print_warning("» Select action (1-6): ");
}
////////////////////////////////////////END OF ADMIN SECTION MENUS/////////////////////////////////////////

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

int insert_semesters(sqlite3 *db) {
    char *err_msg = NULL;
    int rc;
    
    const char *insert_semesters_sql = 
    "INSERT INTO semester (semester_id, enrollment_deadline) VALUES "
    "('S1_2020', '2020-09-01'), "
    "('S2_2021', '2021-02-01'), "
    "('S1_2021', '2021-09-01'), "
    "('S2_2022', '2022-02-01'), "
    "('S1_2022', '2022-09-01'), "
    "('S2_2023', '2023-02-01'), "
    "('S1_2023', '2023-09-01'), "
    "('S2_2024', '2024-02-01'), "
    "('S1_2024', '2024-09-01'), "
    "('S2_2025', '2025-02-01'), "
    "('S1_2025', '2025-09-01'), "
    "('S2_2026', '2026-02-01'), "
    "('S1_2026', '2026-09-01'), "
    "('S2_2027', '2027-02-01'), "
    "('S1_2027', '2027-09-01'), "
    "('S2_2028', '2028-02-01'), "
    "('S1_2028', '2028-09-01'), "
    "('S2_2029', '2029-02-01');";

rc = sqlite3_exec(db, insert_semesters_sql, 0, 0, &err_msg);
if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error inserting semesters: %s\n", err_msg);
    sqlite3_free(err_msg);
    return rc;
}
    
    printf("Semesters inserted successfully!\n");
    return SQLITE_OK;
}

////////////////////////////////////////STUDENT SECTION MENUS/////////////////////////////////////////
void StudentMainMenu() {
    printf("%s", CLEAR_SCREEN);
    print_banner("Student Dashboard", COLOR_GREEN);

    printf("\nAvailable Actions:\n\n");
    print_info("[1] View Personal Information\n");
    print_info("[2] Enroll for Current Academic Year\n");
    print_info("[3] View Enrolled Courses\n");
    print_info("[4] View Grades\n");
    print_info("[5] Logout\n");

    print_warning("» Select action (1-5): ");
}
////////////////////////////////////////END OF STUDENT SECTION MENUS/////////////////////////////////////////


int main() {

    // Enable Unicode support
    enable_unicode_support();

    // Initialize session tracking
    initialize_session();
    
    // Print welcome banner
    print_welcome_banner();
    
    // Database setup
    sqlite3 *univ;
    char *err_msg = 0;

    
    print_info("Initializing database...");
    Sleep(500);
    db_connect(&univ, "univ.db");

    //Delete all existing data in the database for testing purposes
    // sqlite3_exec(univ, "DROP TABLE IF EXISTS student;", NULL, NULL, &err_msg);
    // sqlite3_exec(univ, "DELETE FROM major;", NULL, NULL, &err_msg);
    // sqlite3_exec(univ, "DELETE FROM course;", NULL, NULL, &err_msg);

    CreateStudentTable(univ);
    CreateMajorTable(univ); 
    CreateCourseTable(univ);
    CreateMajorCourseTable(univ);
    create_tables(univ);

    insert_semesters(univ);
    
    // Load data structures
    print_info("Loading student records...");
    Sleep(500);
    Student_List* StudentList = CreateList();
    load_students(univ, StudentList);
    
    //a ajouter apres avoir rempli la table major et la table course
    print_info("Loading major records...");
    Sleep(500);
    Major_List* MajorList = CreateMajorList();
    LoadMajors(univ, MajorList);
    
    print_info("Loading course records...");
    Sleep(500);
    Course_List* CourseList = malloc(sizeof(Course_List));
    CourseList->head = NULL;
    CourseList->num_of_courses = 0;

    print_info("Loading major-course relations...");
    Major* m = MajorList->head;
    while (m != NULL) {
        load_major_courses(univ, MajorList, m->major_id);
        m = m->next;
    }
    
    print_success("System initialization complete!");
    printf("\n%sPress Enter to continue...%s", COLOR_YELLOW, COLOR_RESET);
    getchar();
    
    int exit_program = 0;
    
    while (!exit_program){
        MainMenu();
        
        int choice = get_numeric_input("", 1, 3);
        if (choice == -1) continue;
        
        increment_command_count();
        
        //////////////////////////////////////// START OF ADMIN PORTAL /////////////////////////////////////
        if(choice == 1){
            
            char password[50];
            int attempts = 0;
            int approved = 0;
            
            while (attempts < 3) {
                system("cls");
                print_banner("Admin Login", COLOR_BLUE);
                
                printf("\n%s» Enter admin password:%s ", COLOR_YELLOW, COLOR_RESET);
                get_password(password, sizeof(password));
    
                if (strcmp(password, "admin123") == 0) {
                    approved = 1;
                    print_success("Access granted!");
                    Sleep(1500);
                    break;
                }
    
                attempts++;
                print_error("Wrong password! Try again.");

    
                if (attempts < 3) {
                    printf("%sRemaining tries: %d%s\n", COLOR_YELLOW, 3 - attempts, COLOR_RESET);
                    Sleep(2000);
                }

                else if(attempts >= 3){
                    print_error("Maximum attempts reached. Returning to main menu.");
                    Sleep(2000);
                    MainMenu();
                }
            }

            if (!approved) {
                continue;  
            }

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
                        
                        int student_choice = get_numeric_input("", 1, 8);
                        if (student_choice == -1) continue;
                        
                        increment_command_count();
                        
                        if(student_choice == 1){
                            print_banner("Add New Student", COLOR_GREEN);
                            printf("\n");
                            
                            Student* new_student = CreateNode();
                            Save_student(univ, StudentList, new_student);
                            
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
                            
                            importFromCSV(univ, StudentList, filename);
                            
                            printf("\n%sPress Enter to continue...%s", COLOR_YELLOW, COLOR_RESET);
                            getchar();
                        }
                        else if(student_choice == 3){
                            print_banner("Export Students to CSV", COLOR_GREEN);
                            printf("\n");
                            
                            char filename[100];
                            printf("%s» Enter CSV filename to export to:%s ", COLOR_YELLOW, COLOR_RESET);
                            fgets(filename, sizeof(filename), stdin);
                            filename[strcspn(filename, "\n")] = '\0';
                            
                            print_info("Exporting students...");
                            
                            // progress bar
                            printf("\n");
                            for (int i = 0; i <= 100; i += 5) {
                                display_progress_bar(i, 100, 50);
                                Sleep(50); 
                            }
                            printf("\n");
                            
                            exportToCSV(StudentList, filename);
                            
                            printf("\n%sPress Enter to continue...%s", COLOR_YELLOW, COLOR_RESET);
                            getchar();
                        }
                        else if(student_choice == 4){
                            print_banner("Remove Student", COLOR_RED);
                            printf("\n");
                            
                            int id;
                            printf("%s» Enter Student ID to remove:%s ", COLOR_YELLOW, COLOR_RESET);
                            scanf("%d", &id);
                            clear_input_buffer();
                            
                            print_warning("Removing student...");
                            Sleep(1000);
                            
                            removeStudent(univ, StudentList, id);
                            
                            printf("\n%sPress Enter to continue...%s", COLOR_YELLOW, COLOR_RESET);
                            getchar();
                        }
                        else if(student_choice == 5){
                            print_banner("Update Student", COLOR_YELLOW);
                            printf("\n");
                            
                            int id;
                            printf("%s» Enter Student ID to update:%s ", COLOR_YELLOW, COLOR_RESET);
                            scanf("%d", &id);
                            clear_input_buffer();
                            
                            print_info("Updating student information...");
                            Sleep(1000);
                            
                            updateStudent(univ, MajorList, StudentList, id);
                            print_success("Student updated successfully!");
                            
                            printf("\n%sPress Enter to continue...%s", COLOR_YELLOW, COLOR_RESET);
                            getchar();
                        }
                        else if(student_choice == 6){
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
                        else if(student_choice == 7){
                            print_banner("All Students", COLOR_CYAN);
                            printf("\n");
                            
                            print_info("Loading student list...");
                            Sleep(500);
                            
                            printf("Enter major ID: ");
                            char major_id[50];
                            fgets(major_id, sizeof(major_id), stdin);
                            major_id[strcspn(major_id, "\n")] = '\0';

                            printStudentList(StudentList, MajorList, major_id);
                            
                            printf("\n%sPress Enter to continue...%s", COLOR_YELLOW, COLOR_RESET);
                            getchar();
                        }
                        else if(student_choice == 8){
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
                            
                            SaveCourse(univ, CourseList, new_course);
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
                            
                            removeCourse(univ, CourseList, course_id);
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
                            
                            updateCourse(univ, CourseList, course_id);
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
                            
                            add_course_to_major(univ, MajorList, major_id, course_id, course_name, professor);
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
                            db_insert_major(univ, new_major);
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
                            
                            if(removeMajor(univ, MajorList, major_id)){
                                print_success("Major removed successfully!");
                            } else {
                                print_error("Failed to remove major.");
                            }
                            
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
                            
                            UpdateMajor(univ, MajorList, major_id);
                            print_success("Major updated successfully!");

                            print_info("Updating major...");
                            Sleep(1000);
                            
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
    //////////////////////////////////////// END OF ADMIN PORTAL /////////////////////////////////////




    
    //////////////////////////////////////// START OF STUDENT PORTAL /////////////////////////////////////
    else if(choice == 2){

        int student_id;
        print_warning("\n» Enter your Student ID: ");
        scanf("%d", &student_id);
        clear_input_buffer();

        // Check if student exists
        if(!StudentExistsInDB(univ, student_id)){
            print_error("Student ID not found.");
            Sleep(1500);
            continue;
        }

        print_success("Login successful!");
        Sleep(1000);

        int student_exit = 0;

        while(!student_exit){
            StudentMainMenu();

            int student_choice = get_numeric_input("", 1, 5);
            if (student_choice == -1) continue;

            increment_command_count();

            if(student_choice == 1){
                /******** VIEW PERSONAL INFO ********/
                print_banner("Personal Information", COLOR_CYAN);
                printStudent(StudentList, student_id); // to be modified later 

                print_warning("\nPress Enter to continue...");
                getchar();
            }
            else if(student_choice == 2){
                /******** ENROLLMENT ********/
                print_banner("Academic Enrollment", COLOR_GREEN);

                print_success("Congratulations! You are eligible to enroll for the current academic year.");
                print_info("Proceeding to enrollment...");
                Sleep(2000);

                print_banner("Current Available Majors", COLOR_BLUE);
                if(printAllMajors(MajorList)){
                    print_warning("\n» Enter Major ID to enroll in: ");
                }
                else{
                    print_error("No majors available at the moment. Please contact administration.");
                    print_warning("\nPress Enter to continue...");
                    getchar();
                    continue;
                }

                char major_id[10];
                fgets(major_id, sizeof(major_id), stdin);
                major_id[strcspn(major_id, "\n")] = '\0';
                
                char academic_year[10];
                print_warning("» Enter Academic Year (e.g., 2024-2025): ");
                fgets(academic_year, sizeof(academic_year), stdin);
                academic_year[strcspn(academic_year, "\n")] = '\0'; 
                
                char semester[10];
                print_warning("» Enter Semester (e.g., S1_2024): ");
                fgets(semester, sizeof(semester), stdin);
                semester[strcspn(semester, "\n")] = '\0';
                
                enrollStudent(univ, MajorList, student_id, major_id);
                print_success("Enrollment completed successfully!");
 

                print_warning("\nPress Enter to continue...");
                getchar();
            }
            else if(student_choice == 3){
                /******** VIEW COURSES ********/
                print_banner("Enrolled Courses", COLOR_BLUE);

                Student* student = StudentList -> head;

                while(student != NULL && student -> id != student_id){
                    student = student -> next; 
                }

                if(student -> major_id == NULL){
                    print_error("You are not enrolled in any major. Please enroll first.");
                }
                else{
                    print_info("Fetching your enrolled courses...");
                    Sleep(1000);
                    print_courses_in_major(MajorList, student -> major_id);
                }

                print_warning("\nPress Enter to continue...");
                getchar();
            }
            else if(student_choice == 4){
                /******** VIEW GRADES ********/
                print_banner("Grades & Results", COLOR_MAGENTA);

                print_info("Enter major ID to view grades for (S1_2025): ");
                char major_id[10];
                fgets(major_id, sizeof(major_id), stdin);
                major_id[strcspn(major_id, "\n")] = '\0';
                Sleep(1000);

                print_info("Enter academic year to view grades for (2025-2026): ");
                char academic_year[10];
                fgets(academic_year, sizeof(academic_year), stdin);
                academic_year[strcspn(academic_year, "\n")] = '\0';
                Sleep(1000);

                char semester[10];
                print_warning("» Enter Semester (e.g., S1_2024) or 'all' for all semesters: ");
                fgets(semester, sizeof(semester), stdin);
                semester[strcspn(semester, "\n")] = '\0';
                Sleep(1000);
                
                
                if(!affichage_grade(univ, MajorList, student_id, major_id, semester, academic_year)){
                    print_error("No grades found for the specified semester.");
                }
                else{
                    print_success("Grades fetched successfully!");
                }

                print_warning("\nPress Enter to continue...");
                getchar();
            }
            else if(student_choice == 5){
                print_info("Logging out...");
                Sleep(1000);
                student_exit = 1;
            }
        }
}
//////////////////////////////////////// END OF STUDENT PORTAL /////////////////////////////////////

        else if(choice == 3){
            printf("\n%sExiting program...%s\n", COLOR_YELLOW, COLOR_RESET);
            Sleep(1000);
            exit_program = 1;
        }
    }
    
    // Cleanup and show session summary
    print_info("Closing database connection...");
    Sleep(500);
    sqlite3_close(univ);
    
    print_session_summary();
    
    printf("\n%sPress Enter to exit...%s", COLOR_YELLOW, COLOR_RESET);
    getchar();
    
    return 0;
}