#ifndef UI_HELPERS_H
#define UI_HELPERS_H

#include <stdio.h>
#include <time.h>
#include <string.h>

// ANSI color codes for Windows (works in modern Windows Terminal)
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[91m"
#define COLOR_GREEN   "\033[92m"
#define COLOR_YELLOW  "\033[93m"
#define COLOR_BLUE    "\033[94m"
#define COLOR_MAGENTA "\033[95m"
#define COLOR_CYAN    "\033[96m"
#define COLOR_WHITE   "\033[97m"
#define BOLD          "\033[1m"
#define CLEAR_SCREEN  "\033[2J\033[H"

// Status icons
#define ICON_SUCCESS "✓"
#define ICON_ERROR   "✗"
#define ICON_INFO    "→"
#define ICON_WARNING "⚠"

// Helper functions for colored output
void print_colored(const char* text, const char* color);
void print_success(const char* message);
void print_error(const char* message);
void print_info(const char* message);
void print_warning(const char* message);
void print_banner(const char* text, const char* color);

// Progress bar
void display_progress_bar(long current, long total, int bar_width);
void display_progress_percentage(long current, long total);

// Welcome banner
void print_welcome_banner(void);

// Session summary
typedef struct {
    int commands_executed;
    int students_added;
    int students_imported;
    int students_removed;
    int courses_added;
    int majors_added;
    int errors_encountered;
    time_t start_time;
    time_t end_time;
} SessionStats;

void initialize_session(void);
void increment_command_count(void);
void increment_student_added(void);
void increment_student_imported(void);
void increment_course_added(void);
void increment_major_added(void);
void increment_error_count(void);
void print_session_summary(void);

// Autocomplete helpers
void show_command_suggestions(const char* partial);
void clear_input_buffer(void);
int get_numeric_input(const char* prompt, int min, int max);

#endif