#include "ui_helpers.h"
#include <stdlib.h>
#include <windows.h>

// Global session statistics
static SessionStats session = {0};

// Colored output functions
void print_colored(const char* text, const char* color) {
    printf("%s%s%s", color, text, COLOR_RESET);
}

void print_success(const char* message) {
    printf("%s%s %s%s\n", COLOR_GREEN, ICON_SUCCESS, message, COLOR_RESET);
}

void print_error(const char* message) {
    printf("%s%s %s%s\n", COLOR_RED, ICON_ERROR, message, COLOR_RESET);
}

void print_info(const char* message) {
    printf("%s%s %s%s\n", COLOR_CYAN, ICON_INFO, message, COLOR_RESET);
}

void print_warning(const char* message) {
    printf("%s%s %s%s\n", COLOR_YELLOW, ICON_WARNING, message, COLOR_RESET);
}

void print_banner(const char* text, const char* color) {
    printf("\n\n");
    int len = strlen(text);
    int padding = (60 - len) / 2;
    
    printf("\n%s", color);
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                                                              ║\n");
    printf("║%*s%s%*s  ║\n", padding, "", text, padding + (len % 2 == 0 ? 0 : 1), "");
    printf("║                                                              ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("%s", COLOR_RESET);
}

// Progress bar
void display_progress_bar(long current, long total, int bar_width) {
    if (total == 0) return;
    
    float percentage = (float)current / total;
    int progress = percentage * bar_width;
    
    printf("\r[");
    for (int i = 0; i < bar_width; i++) {
        if (i < progress) {
            printf("%s█%s", COLOR_GREEN, COLOR_RESET);
        } else {
            printf(" ");
        }
    }
    
    printf("] %s%3.0f%%%s (%ld/%ld)", 
           COLOR_CYAN, percentage * 100, COLOR_RESET, current, total);
    fflush(stdout);
}

void display_progress_percentage(long current, long total) {
    if (total == 0) return;
    
    float percentage = (float)current / total * 100;
    printf("\r%sProgress:%s %s%5.1f%%%s", 
           COLOR_BLUE, COLOR_RESET, COLOR_CYAN, percentage, COLOR_RESET);
    fflush(stdout);
}

// Welcome banner
void print_welcome_banner(void) {
    printf("\n\n");
    system("cls");
    
    // Get current time
    SYSTEMTIME st;
    GetLocalTime(&st);
    const char* greeting;
    
    if (st.wHour < 12 && st.wHour >= 6) greeting = "Good morning";
    else if (st.wHour < 18 && st.wHour >= 12) greeting = "Good afternoon";
    else greeting = "Good evening";
    
    printf("\n%s", COLOR_BLUE);
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                                                              ║\n");
    printf("║           STUDENT MANAGEMENT SYSTEM v2.0                     ║\n");
    printf("║                                                              ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("%s", COLOR_RESET);
    
    printf("\n%s%s! Welcome Administrator%s\n\n", 
           COLOR_GREEN, greeting, COLOR_RESET);
    
    printf("%s» Type 'menu' to see available commands%s\n", 
           COLOR_YELLOW, COLOR_RESET);
    printf("%s» Use 'quit' to exit the program%s\n\n", 
           COLOR_YELLOW, COLOR_RESET);
}

// Session management
void initialize_session(void) {
    session.start_time = time(NULL);
    session.commands_executed = 0;
    session.students_added = 0;
    session.students_imported = 0;
    session.students_removed = 0;
    session.courses_added = 0;
    session.majors_added = 0;
    session.errors_encountered = 0;
}

void increment_command_count(void){
    session.commands_executed++;
}

void increment_student_added(void){
    session.students_added++;
}

void increment_student_imported(void){
    session.students_imported++;
}

void increment_student_exported(void){
    session.students_exported++;
}

void increment_course_added(void){
    session.courses_added++;
}

void increment_major_added(void){
    session.majors_added++;
}

void increment_error_count(void){
    session.errors_encountered++;
}

void print_session_summary(void){
    session.end_time = time(NULL);
    double duration = difftime(session.end_time, session.start_time);
    
    printf("\n%s", COLOR_CYAN);
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                                                              ║\n");
    printf("║                     SESSION SUMMARY                          ║\n");
    printf("║                                                              ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("%s", COLOR_RESET);
    
    printf("%s║  Commands executed:                                     %5d║%s\n", COLOR_CYAN, session.commands_executed, COLOR_RESET);
    printf("%s║  Students added:                                        %5d║%s\n", COLOR_CYAN, session.students_added, COLOR_RESET);
    printf("%s║  Students imported:                                     %5d║%s\n", COLOR_CYAN, session.students_imported, COLOR_RESET);
    printf("%s║  Students exported:                                     %5d║%s\n", COLOR_CYAN, session.students_exported, COLOR_RESET);
    printf("%s║  Students removed:                                      %5d║%s\n", COLOR_CYAN, session.students_removed, COLOR_RESET);
    printf("%s║  Courses added:                                         %5d║%s\n", COLOR_CYAN, session.courses_added, COLOR_RESET);
    printf("%s║  Majors added:                                          %5d║%s\n", COLOR_CYAN, session.majors_added, COLOR_RESET);
    printf("%s║  Errors encountered:                                    %5d║%s\n", COLOR_CYAN, session.errors_encountered, COLOR_RESET);
    
    // Calculate minutes and seconds
    int minutes = (int)duration / 60;
    int seconds = (int)duration % 60;
    printf("%s║  Session duration:                                      %02d:%02d║%s\n", COLOR_CYAN, minutes, seconds, COLOR_RESET);
    
    printf("%s", COLOR_CYAN);
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("%s", COLOR_RESET);
    
    // Performance feedback
    printf("\n");
    if (session.errors_encountered == 0 && session.commands_executed > 0) {
        print_success("Flawless execution! All operations completed successfully.");
    } else if (session.errors_encountered < 3) {
        print_success("Good session! Minor issues were resolved.");
    } else {
        print_warning("Session completed with some challenges to learn from.");
    }
    
    printf("\n%sThank you for using Student Management System. Goodbye!%s\n", 
           COLOR_MAGENTA, COLOR_RESET);
}

// might use later for input handling
void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int get_numeric_input(const char* prompt, int min, int max) {
    int value;
    char input[100];
    
    while (1) {//infinite loop until valid input
        printf("%s", prompt);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {//user pressed Ctrl+D or Ctrl+C or Ctrl+Z or error. We also use fgets to avoid buffer overflow and because scanf leaves newline in buffer
            return -1;
        }
        
        if (sscanf(input, "%d", &value) == 1 && value >= min && value <= max) {//sscanf checks the first item in input and sees if it is an integer and within range. sscanf returns number of successfully matched items
            return value; //i,n this case sscanf will always either return 1 or 0
        }
        
        print_error("Invalid input. Please try again.");
        printf("Enter a number between %d and %d: ", min, max);
    }
}