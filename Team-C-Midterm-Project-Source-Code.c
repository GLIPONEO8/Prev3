#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <ctype.h>
#include <conio.h>

#define GREEN "\x1b[38;5;22m"
#define BLUE "\x1b[38;5;20m"
#define RED "\x1b[38;5;88m"
#define WHITE "\x1b[38;5;255m"
#define WHITEBG "\x1b[48;5;255m"
#define BLACKBG "\x1b[40m"
#define NORMAL "\x1b[30m"

#define FOLDER_PATH "tupmanila_data" // Name of the folder to create

#define CHAR_LIMIT 15    // Character limit for credentials
#define MAX_SIZE 20     // For username, password
#define MAX_NAME 100     // Maximum length of names
#define MAX_QSCORE 100   // Qty. of Quiz that can be added
#define MAX_STUDENT 100  // Qty. of Students that can be added
#define MAX_REQ 5        // Qty. of Students required in record
#define MAX_BSIZE 100    // Maximum size for encryption buffer
#define MAX_TOP 5        // Qty. of top students to display

struct STUDENT
{
    char name[MAX_NAME];
    int attendance;
    int quiz_scores[MAX_QSCORE];
    int major_exam_scores[MAX_QSCORE];
    int is_occupied;
};

struct CLASS
{
    int size;

    int quiz_total[MAX_QSCORE];
    int quiz_percentage;
    int quiz_count;

    int major_exam_total[MAX_QSCORE];
    int major_exam_percentage;
    int major_exam_count;

    int attendance_percentage;
    int attendance_total;
};

// Global Variables

char current_dir[MAX_PATH];
char full_folder_path[MAX_PATH];

// Prototypes

void log_in_screen(void);
void sign_up_screen(void);
void user_panel_screen(const char *username, const char *filename);
void manage_record_data_screen(struct CLASS *class_data, struct STUDENT *students);
void edit_percentage_distribution_screen(struct CLASS *class_data);
void manage_attendance_screen(struct CLASS *class_data, struct STUDENT *students);
void edit_total_attendance_screen(struct CLASS *class_data, struct STUDENT *students);
void edit_individual_attendance_screen(struct CLASS *class_data, struct STUDENT *students);
void edit_student_name_screen(struct STUDENT *students, int size);
void manage_quiz_score_screen(struct CLASS *class_data, struct STUDENT *students);
void edit_total_quiz_score_screen(struct CLASS *class_data, struct STUDENT *students);
void edit_individual_quiz_score_screen(struct CLASS *class_data, struct STUDENT *students);
void edit_quiz_count_screen(struct CLASS *class_data, struct STUDENT *students);
void manage_major_exam_score_screen(struct CLASS *class_data, struct STUDENT *students);
void edit_total_major_exam_score_screen(struct CLASS *class_data, struct STUDENT *students);
void edit_individual_major_exam_score_screen(struct CLASS *class_data, struct STUDENT *students);
void edit_major_exam_count_screen(struct CLASS *class_data, struct STUDENT *students);
void general_student_assessment_screen(struct CLASS class_data, struct STUDENT *students);
void individual_student_assessment_screen(struct CLASS class_data, struct STUDENT *students);
void remove_student_screen(struct CLASS *class_data, struct STUDENT *students);
void add_student_screen(struct CLASS *class_data, struct STUDENT *students);
int is_alphanumeric(const char *str);
int is_not_excess(char *str);
int get_int(void);
int check_account(char *filename);
int check_directory(const char* path);
void get_credential(char *dest);
int create_account(const char *encrypted_filename);
void create_data_folder(void);
void encrypt_text(const char *username, const char *password, char *dest);
void remove_newline(char *str);
void load_file_data(struct CLASS *class_data, struct STUDENT *students, const char *filename);
void save_data(struct CLASS *class_data, struct STUDENT *students, const char *filename);
void rearrange_array(struct STUDENT *students, int size);
float compute_grade(struct CLASS class_data, struct STUDENT *students, int i);
int sum_array(int *arr, int size);
void class_mean_sort_descending(float *arr, int *ndx, int size);
float get_highest_rating(int *scores, int *total_scores ,int *item, int size);
float get_lowest_rating(int *scores, int *total_scores ,int *item, int size);
void display_title(void);
void display_individual_student_data(struct CLASS class_data, struct STUDENT *students);
void display_student_list(struct STUDENT *students, int size);
void display_grade_categories(void);
void get_log_in_credential(char *dest);
void about_screen(void);


// Main

int main()
{
    char action;
    int success;

    system("COLOR F0");
    create_data_folder();

    while(1)
    {
        display_title();
        printf("  Press the corresponding key for the action you want to select:\n\n");
        printf("    %s[L]%s - LOG IN\n", BLUE, NORMAL);
        printf("    %s[S]%s - SIGN UP\n", BLUE, NORMAL);
        printf("    %s[A]%s - ABOUT\n", BLUE, NORMAL);
        printf("    %s[E]%s - EXIT\n\n", BLUE, NORMAL);
        printf("   %sYOUR CHOICE: %s", RED, NORMAL);

        action = toupper(getch());
        switch(action)
        {
            case 'L': log_in_screen(); break;
            case 'S': sign_up_screen(); break;
            case 'A': about_screen(); break;
            case 'E': printf("\n\n\n"); exit(1); break;
        }
    }
}

// Functions that allow for user interaction


void log_in_screen(void)
{
    char encryption_buffer[MAX_BSIZE];
    char username[MAX_SIZE];
    char password[MAX_SIZE];
    char ch;

    int i = 0;
    int s;

    fflush(stdin);
    display_title();

    printf("  %sMENU > LOG IN%s\n\n", RED, NORMAL);
    printf("    %sFORMAT:%s Username/Password length must be ALPHANUMERIC, with length of 1 to 15.\n    (For example: %sIvan123%s)", BLUE, NORMAL, BLUE, NORMAL);

    printf("\n\n    [USERNAME]: %s", BLUE);
    get_log_in_credential(username);
    printf("%s", NORMAL);

    printf("    [PASSWORD]: ");

    while ((ch = getch()) != '\r')
    {
        if (ch == '\b')
        {
            if (i > 0)
            {
                printf("\b \b");
                i--;
            }
        }
        else
        {
            if (i < MAX_SIZE - 1)
            {
                password[i] = ch;
                printf("%s*%s", BLUE, NORMAL);
                i++;
            }
        }
    }

    password[i] = '\0'; // Terminate the string with a null character

    if (username[0] == '\n' || username[0] == '\0' || password[0] == '\n' || password[0] == '\0')
    {
        printf("\n\n    %s[!] Your input is invalid. Make sure to follow the format and try again.%s", RED, NORMAL);
        getch();
        return;
    }

    encrypt_text(username, password, encryption_buffer);
    strcat(encryption_buffer, ".rcd");

    if ((is_alphanumeric(username) && is_alphanumeric(password)) && check_account(encryption_buffer))
    {
        user_panel_screen(username, encryption_buffer);
    }
    else
    {
        printf("\n\n    %s[!] Your credentials are invalid. Please check and try again.%s\n    ", RED, NORMAL);
        getch();
        return;
    }
}

void sign_up_screen(void)
{
    char username[MAX_SIZE];
    char password[MAX_SIZE];
    char encryption_buffer[MAX_BSIZE];

    fflush(stdin);
    display_title();

    printf("  %sMENU > SIGN UP%s\n\n", RED, NORMAL);
    printf("    FORMAT: Username/Password length must be ALPHANUMERIC, with length of 1 to 15.\n    (For example: %sIvan123%s)", BLUE, NORMAL);

    printf("\n\n    [USERNAME]: %s", BLUE);
    get_credential(username);
    printf("%s", NORMAL);
    fflush(stdin);

    printf("    [PASSWORD]: %s", BLUE);
    get_credential(password);
    printf("%s", NORMAL);
    fflush(stdin);

    if (username[0] == '\n' || username[0] == '\0' || password[0] == '\n' || password[0] == '\0')
    {
        printf("\n    %s[!] Your input is invalid. Make sure to follow the format and try again.%s", RED, NORMAL);
        getch();
        return;
    }

    if (is_alphanumeric(username) && is_not_excess(username) && is_alphanumeric(password) && is_not_excess(password))
    {
        encrypt_text(username, password, encryption_buffer);
        strcat(encryption_buffer, ".rcd");

        if (check_account(encryption_buffer))
        {
            printf("\n    %s[!] Your account already exists.", RED);
            printf("\n    [!] Try logging in by pressing any key to return to the main menu.%s\n    ", NORMAL);

            getch();
            return;
        }
        create_account(encryption_buffer);

        system("cls");
        display_title();

        printf("  %sMENU > SIGN UP%s\n ", RED, NORMAL);
        printf("\n    %s[!] Congratulations! Your account has been created, please REMEMBER the following credentials:%s ", GREEN, NORMAL);

        printf("\n\n    USERNAME: %s%s%s ", BLUE, username, NORMAL);
        printf("\n    PASSWORD: %s%s%s   ", BLUE, password, NORMAL);

        printf("\n\n    Log in by pressing any key to return to the main menu.\n    ");
        getch();
    }
    else
    {
        printf("\n    %s[!] Your input is invalid. Make sure to follow the format and try again.%s", RED, NORMAL);
        getch();
        return;
    }
}

void about_screen(void)
{
    display_title();
    printf("\n\tThis improved student assessment offers a range of enhanced features. Firstly, we have implemented a new login"
           "\n\tfeature that enables users to create an account and securely store their student records. Secondly, we have"
           "\n\tincorporated an attendance feature that meticulously tracks students' attendance and tardiness.\n\n"
           "\tThe third feature focuses on quizzes, including the ability to record quiz results and a ranking system that"
           "\n\torders them by score. Additionally, we have integrated a major exam feature to assess students' performance"
           "\n\ton important assessments. Lastly, we have introduced a report generation feature that informs users of the"
           "\n\toverall scores and attendance status of the students, indicating whether they have passed or failed.\n\n"
           "\tInstructions:\n"
           "\tIf you do not have an account yet, press S to sign up. Enter a username and password. Then, press Enter to"
           "\n\treturn to the main menu. Press L to log in. Enter the same username and password that you used to sign up.\n\n"
           "\tTo see the general student assessment, press G. To see the individual assessment, press I. To add a student,"
           "\n\tpress A. The system will automatically add the student. To remove a student, press R. Select a student number"
           "\n\tand press Enter to remove the student from the list.\n\n"
           "\t[!] Press any key to return to menu...\n\t");
           getch();
           return;
}

void user_panel_screen(const char *username, const char *filename)
{
    char action;
    int s;

    struct STUDENT students[MAX_STUDENT] = {0};
    struct CLASS class_data = {0};

    load_file_data(&class_data, students, filename);
    fflush(stdin);

    while(1)
    {
        display_title();

        printf("  %sMENU > LOG IN > USER PANEL%s\n\n", RED, NORMAL);
        printf("  Welcome %s%s%s!\n  Press the corresponding key for the action you want to select:\n\n", BLUE, username, NORMAL);

        printf("    %s[G]%s - GENERAL STUDENT ASSESSMENT\n", BLUE, NORMAL);
        printf("    %s[I]%s - INDIVIDUAL STUDENT ASSESSMENT\n", BLUE, NORMAL);
        printf("    %s[A]%s - ADD STUDENT\n", BLUE, NORMAL);
        printf("    %s[R]%s - REMOVE STUDENT\n", BLUE, NORMAL);
        printf("    %s[M]%s - MANAGE RECORD DATA\n", BLUE, NORMAL);
        printf("    %s[S]%s - DISPLAY INDIVIDUAL STUDENT DATA\n", BLUE, NORMAL);
        printf("    %s[L]%s - SAVE AND LOG OUT\n\n", BLUE, NORMAL);

        printf("   %sYOUR CHOICE: %s", RED, NORMAL);
        action = toupper(getch());

        switch(action)
        {
            case 'G': general_student_assessment_screen(class_data, students); break;
            case 'I': individual_student_assessment_screen(class_data, students); break;
            case 'A': add_student_screen(&class_data, students); break;
            case 'R': remove_student_screen(&class_data, students); break;
            case 'M': manage_record_data_screen(&class_data, students); break;
            case 'S': display_individual_student_data(class_data, students); break;
            case 'L': save_data(&class_data, students, filename); return;
        }
    }
}

void manage_record_data_screen(struct CLASS *class_data, struct STUDENT *students)
{
    char choice;
    while(1)
    {
        fflush(stdin);
        display_title();

        printf("  %sMENU > LOG IN > USER PANEL > MANAGE RECORD DATA%s\n\n", RED, NORMAL);
        printf("  Press the corresponding key for the action you want to select:\n\n");

        printf("    %s[P]%s - EDIT PERCENTAGE DISTRIBUTION\n", BLUE, NORMAL);
        printf("    %s[C]%s - CHANGE STUDENT NAME\n", BLUE, NORMAL);
        printf("    %s[A]%s - MANAGE ATTENDANCE\n", BLUE, NORMAL);
        printf("    %s[Q]%s - MANAGE QUIZZES\n", BLUE, NORMAL);
        printf("    %s[M]%s - MANAGE MAJOR EXAM\n", BLUE, NORMAL);
        printf("    %s[R]%s - RETURN\n\n", BLUE, NORMAL);

        printf("   %sYOUR CHOICE: %s", RED, NORMAL);

        choice = toupper(getch());

        switch (choice)
        {

            case 'P':
                    edit_percentage_distribution_screen(class_data);
                    break;

            case 'C':
                    edit_student_name_screen(students, class_data->size);
                    break;

            case 'A':
                    manage_attendance_screen(class_data, students);
                    break;

            case 'Q':
                    manage_quiz_score_screen(class_data, students);
                    break;

            case 'M':
                    manage_major_exam_score_screen(class_data, students);
                    break;

            case 'R': return;
        }
    }
}

void edit_percentage_distribution_screen(struct CLASS *class_data)
{
    int temp_attendance_percentage;
    int temp_quiz_percentage;
    int temp_major_exam_percentage;

    fflush(stdin);
    system("cls");
    display_title();

    printf("  %sMENU > LOG IN > USER PANEL > MANAGE RECORD DATA > EDIT PERCENTAGE DISTRIBUTION%s\n\n", RED, NORMAL);
    printf("    PERCENTAGE DISTRIBUTION:\n\n");
    printf("      ATTENDANCE : %s%d%%%s\n", BLUE, class_data->attendance_percentage, NORMAL);
    printf("      QUIZ       : %s%d%%%s\n", BLUE, class_data->quiz_percentage, NORMAL);
    printf("      MAJOR EXAM : %s%d%%%s\n\n", BLUE, class_data->major_exam_percentage, NORMAL);
    printf("    Enter the percentage for each category:\n\n");

    printf("    [ATTENDANCE]:   %s", BLUE);
    temp_attendance_percentage = get_int();
    printf("%s", NORMAL);

    printf("    [QUIZZES]:      %s", BLUE);
    temp_quiz_percentage = get_int();
    printf("%s", NORMAL);

    printf("    [MAJOR EXAMS]:  %s", BLUE);
    temp_major_exam_percentage = get_int();
    printf("%s", NORMAL);

    if (temp_attendance_percentage + temp_quiz_percentage + temp_major_exam_percentage == 100)
    {
        class_data->attendance_percentage = temp_attendance_percentage;
        class_data->quiz_percentage = temp_quiz_percentage;
        class_data->major_exam_percentage = temp_major_exam_percentage;

        printf("\n    %s[!] Percentage distribition has been updated successfully!\n", GREEN);
        printf("    [!] Press any key to return...%s\n    ", NORMAL);
        getch();

        return;
    }
    else
    {
        printf("\n    %s[!] The percentage distribution is INVALID or does not ADD UP to 100.\n", RED);
        printf("    [!] Press any key to return...%s\n    ", NORMAL);
        getch();

        return;
    }
}

void manage_attendance_screen(struct CLASS *class_data, struct STUDENT *students)
{
    int choice;

    while(1)
    {
        fflush(stdin);
        display_title();
        printf("  %sMENU > LOG IN > USER PANEL > MANAGE RECORD DATA > MANAGE ATTENDANCE%s\n\n", RED, NORMAL);
        printf("  Press the corresponding key for the action you want to select:\n\n");

        printf("    %s[T]%s - EDIT TOTAL ATTENDANCE\n", BLUE, NORMAL);
        printf("    %s[I]%s - EDIT STUDENT ATTENDANCE\n", BLUE, NORMAL);
        printf("    %s[R]%s - RETURN\n\n", BLUE, NORMAL);

        printf("   %sYOUR CHOICE: %s", RED, NORMAL);

        choice = toupper(getch());

        switch (choice)
        {
            case 'T':
                edit_total_attendance_screen(class_data, students);
                break;

            case 'I':
                edit_individual_attendance_screen(class_data, students);
                break;

            case 'R': return;
        }
    }
}

void edit_total_attendance_screen(struct CLASS *class_data, struct STUDENT *students)
{
    int temp_data, s;

    fflush(stdin);
    display_title();
    printf("  %sMENU > LOG IN > USER PANEL > MANAGE RECORD DATA > MANAGE ATTENDANCE > EDIT TOTAL ATTENDANCEs\n\n", RED, NORMAL);
    printf("    %sNote:%s The attendance of all students will RESET to 0 if attendance is changed\n\n", RED, NORMAL);
    printf("    TOTAL ATTENDANCE: %s", BLUE);
    printf("%s", NORMAL);

    temp_data = get_int();

    if (temp_data == -1)
    {
        printf("\n    %s[!] Invalid Input. Please try again...%s", RED, NORMAL);
        getch();
        return;
    }

    class_data->attendance_total = temp_data;

    for (s = 0; s < class_data->size; s++) // Reset the specific quiz score of students to 0
    {
        students[s].attendance = 0;
    }

    printf("\n    %s[!] Total Attendance has been updated successfully.\n", GREEN);
    printf("    [!] Press any key to return...%s\n    ", NORMAL);
    getch();
}

void edit_individual_attendance_screen(struct CLASS *class_data, struct STUDENT *students)
{
    const int size = class_data->size;
    int temp_data;
    int id;

    fflush(stdin);
    display_title();
    printf("  %sMENU > LOG IN > USER PANEL > MANAGE RECORD DATA > MANAGE ATTENDANCE > EDIT STUDENT ATTENDANCE%s\n\n", RED, NORMAL);
    display_student_list(students, size);
    printf("\n    WHICH STUDENT NUMBER TO EDIT: %s", BLUE);

    id = get_int();
    printf("%s", NORMAL);

    if (id < 1 || id > size) // Premature exit
    {
        printf("\n    %s[!] Invalid student number. Please try again.%s\n    ", RED, NORMAL);
        getch();
        return;
    }

    printf("\n    EDIT ATTENDANCE OF STUDENT #%d - %s%s%s\n", id, BLUE, students[id-1].name, NORMAL);
    printf("\n      ATTENDANCE: %s", BLUE);
    temp_data = get_int();
    printf("%s", NORMAL);

    if (temp_data < 0 || temp_data > class_data->attendance_total)
    {
        printf("\n    %s[!] Invalid input. Please try again...\n    %s", RED, NORMAL);
        return;
    }

    students[id-1].attendance = temp_data;

    printf("\n    %s[!] Attendance of Student #%d has been edited successfully.\n", GREEN, id);
    printf("    [!] Press any key to return...%s\n    ", NORMAL);
    getch();
}

void edit_student_name_screen(struct STUDENT *students, int size)
{
    char temp_data[MAX_BSIZE];
    int id;

    fflush(stdin);
    display_title();
    printf("  %sMENU > LOG IN > USER PANEL > MANAGE RECORD DATA > MANAGE MAJOR EXAM > CHANGE STUDENT NAME%s\n\n", RED, NORMAL);
    display_student_list(students, size);
    printf("\n    WHICH STUDENT NUMBER TO EDIT: %s", BLUE);

    id = get_int();
    printf("%s", NORMAL);

    if (id < 1 || id > size) // Premature exit
    {
        printf("\n    %s[!] Invalid student number. Please try again.%s\n    ", RED, NORMAL);
        getch();
        return;
    }

    printf("\n    EDIT NAME OF STUDENT #%d - %s%s%s\n", id, BLUE, students[id-1].name, NORMAL);
    printf("    CHANGE TO: %s", BLUE);
    fgets(temp_data, MAX_BSIZE, stdin);
    printf("%s", NORMAL);
    remove_newline(temp_data);

    strcpy(students[id-1].name, temp_data);

    printf("\n    %s[!] Name of Student #%d has been edited successfully.\n", GREEN, id);
    printf("    [!] Press any key to return...%s\n    ", NORMAL);
    getch();
}

void manage_quiz_score_screen(struct CLASS *class_data, struct STUDENT *students)
{
    int choice;

    while(1)
    {
        fflush(stdin);
        display_title();
        printf("  %sMENU > LOG IN > USER PANEL > MANAGE RECORD DATA > MANAGE QUIZZES%s\n\n", RED, NORMAL);
        printf("  Press the corresponding key for the action you want to select:\n\n");

        printf("    %s[T]%s - EDIT TOTAL SCORES\n", BLUE, NORMAL);
        printf("    %s[I]%s - EDIT STUDENT SCORES\n", BLUE, NORMAL);
        printf("    %s[C]%s - EDIT QUIZ COUNT\n", BLUE, NORMAL);
        printf("    %s[R]%s - RETURN\n\n", BLUE, NORMAL);

        printf("   %sYOUR CHOICE: %s", RED, NORMAL);

        choice = toupper(getch());

        switch (choice)
        {
            case 'T':
                edit_total_quiz_score_screen(class_data, students);
                break;
            case 'I':
                edit_individual_quiz_score_screen(class_data, students);
                break;

            case 'C':
                edit_quiz_count_screen(class_data, students);
                break;

            case 'R': return;
        }
    }
}

void edit_total_quiz_score_screen(struct CLASS *class_data, struct STUDENT *students)
{
    int temp_data, i, s;

    fflush(stdin);
    display_title();
    printf("  %sMENU > LOG IN > USER PANEL > MANAGE RECORD DATA > MANAGE QUIZZES > EDIT TOTAL SCORES%s\n\n", RED, NORMAL);
    printf("    %sNote:%s If the total score is changed, all scores of the students in that quiz will RESET to 0.\n\n", BLUE, NORMAL);
    for (i = 0; i < class_data->quiz_count; i++)
    {
        printf("    TOTAL SCORE FOR QUIZ #%d: %s", (i+1), BLUE);
        temp_data = get_int();
        printf("%s", NORMAL);

        if (temp_data > 0)
        {
            if (temp_data != class_data->quiz_total[i])
            {
                class_data->quiz_total[i] = temp_data;
                for (s = 0; s < class_data->size; s++) // Reset the scores of students to 0
                {
                    students[s].quiz_scores[i] = 0;
                }
            }
        }
        else
        {
            printf("\n    %s[!] You entered an invalid score. Please try again.%s\n    ", RED, NORMAL);
            getch();
            return;
        }
    }
    printf("\n    %s[!] Total Quiz Scores have been edited successfully.\n", GREEN);
    printf("    [!] Press any key to return...%s\n    ", NORMAL);
    getch();
}

void edit_individual_quiz_score_screen(struct CLASS *class_data, struct STUDENT *students)
{
    int temp_data, id, q;

    fflush(stdin);
    display_title();
    printf("  %sMENU > LOG IN > USER PANEL > MANAGE RECORD DATA > MANAGE QUIZZES > EDIT STUDENT SCORES%s\n\n", RED, NORMAL);
    display_student_list(students, class_data->size);
    printf("\n    WHICH STUDENT NUMBER TO EDIT: %s", BLUE);

    id = get_int();
    printf("%s", NORMAL);

    if (id < 1 || id > class_data->size) // Premature exit
    {
        printf("\n    %s[!] Invalid student number. Please try again.%s\n    ", RED, NORMAL);
        getch();
        return;
    }

    printf("\n    EDIT QUIZ RECORD OF STUDENT #%d - %s%s%s\n\n", id, BLUE, students[id-1].name, NORMAL);
    for (q = 0; q < class_data->quiz_count; q++)
    {
        printf("      QUIZ %d: %s", (q+1), BLUE);
        temp_data = get_int();
        printf("%s", NORMAL);

        if (temp_data < 0 || temp_data > class_data->quiz_total[q])
        {
            printf("\n      %s[!] You entered an invalid score. Please try again.%s\n      ", RED, NORMAL);
            getch();
            return;
        }
        else
        {
            students[id-1].quiz_scores[q] = temp_data;
        }
    }
    printf("\n      %s[!] Quiz scores of Student #%d have been edited successfully.\n", GREEN, id);
    printf("      [!] Press any key to return...%s\n      ", NORMAL);
    getch();
}

void edit_quiz_count_screen(struct CLASS *class_data, struct STUDENT *students)
{
    int temp_data;

    fflush(stdin);
    display_title();
    printf("  %sMENU > LOG IN > USER PANEL > MANAGE RECORD DATA > MANAGE QUIZZES > EDIT QUIZ COUNT%s\n\n", RED, NORMAL);
    printf("    %sNote:%s The number of quizzes must be AT LEAST One (1)\n\n", RED, NORMAL);
    printf("    NUMBER OF QUIZZES: %s", BLUE);
    temp_data = get_int();
    printf("%s", NORMAL);

    if (temp_data < 1)
    {
        printf("\n    %s[!] Invalid input. Please try again.%s\n    ", RED, NORMAL);
        getch();
        return;
    }
    else
        class_data->quiz_count = temp_data;

    printf("\n    %s[!] Quiz count has been updated successfully.\n", GREEN);
    printf("    [!] Press any key to return...%s\n    ", NORMAL);
    getch();
}

void manage_major_exam_score_screen(struct CLASS *class_data, struct STUDENT *students)
{
    int choice;



    while(1)
    {
        fflush(stdin);
        display_title();
        printf("  %sMENU > LOG IN > USER PANEL > MANAGE RECORD DATA > MANAGE MAJOR EXAM%s\n\n", RED, NORMAL);
        printf("  Press the corresponding key for the action you want to select:\n\n");

        printf("    %s[T]%s - EDIT TOTAL SCORES\n", BLUE, NORMAL);
        printf("    %s[I]%s - EDIT STUDENT SCORES\n", BLUE, NORMAL);
        printf("    %s[C]%s - EDIT MAJOR EXAM COUNT\n", BLUE, NORMAL);
        printf("    %s[R]%s - RETURN\n\n", BLUE, NORMAL);

        printf("   %sYOUR CHOICE: %s", RED, NORMAL);

        choice = toupper(getch());

        switch (choice)
        {
            case 'T':
                edit_total_major_exam_score_screen(class_data, students);
                break;
            case 'I':
                edit_individual_major_exam_score_screen(class_data, students);
                break;

            case 'C':
                edit_major_exam_count_screen(class_data, students);
                break;

            case 'R': return;
        }
    }
}

void edit_total_major_exam_score_screen(struct CLASS *class_data, struct STUDENT *students)
{
    int temp_data, i, s;

    fflush(stdin);
    display_title();
    printf("  %sMENU > LOG IN > USER PANEL > MANAGE RECORD DATA > MANAGE MAJOR EXAM > EDIT TOTAL SCORES%s\n\n", RED, NORMAL);
    printf("    %sNote:%s If the total score is changed, all scores of the students in that major exam will RESET to 0.\n\n", BLUE, NORMAL);
    for (i = 0; i < class_data->major_exam_count; i++)
    {
        printf("    TOTAL SCORE FOR MAJOR EXAM #%d: %s", (i+1), BLUE);
        temp_data = get_int();
        printf("%s", NORMAL);

        if (temp_data > 0)
        {
            if (temp_data != class_data->major_exam_total[i])
            {
                class_data->major_exam_total[i] = temp_data;
                for (s = 0; s < class_data->size; s++) // Reset the specific quiz score of students to 0
                {
                    students[s].major_exam_scores[i] = 0;
                }
            }
        }
        else
        {
            printf("\n    %s[!] You entered an invalid score. Please try again.%s\n    ", RED, NORMAL);
            getch();
            return;
        }
    }
    printf("\n    %s[!] Total Major Exam Scores have been edited successfully.\n", GREEN);
    printf("    [!] Press any key to return...%s\n    ", NORMAL);
    getch();
}

void edit_individual_major_exam_score_screen(struct CLASS *class_data, struct STUDENT *students)
{
    int temp_data, id, m;

    fflush(stdin);
    display_title();
    printf("  %sMENU > LOG IN > USER PANEL > MANAGE RECORD DATA > MANAGE MAJOR EXAM > EDIT STUDENT SCORES%s\n\n", RED, NORMAL);
    display_student_list(students, class_data->size);
    printf("\n    WHICH STUDENT NUMBER TO EDIT: %s", BLUE);

    id = get_int();
    printf("%s", NORMAL);

    if (id < 1 || id > class_data->size) // Premature exit
    {
        printf("\n    %s[!] Invalid student number. Please try again.%s\n    ", RED, NORMAL);
        getch();
        return;
    }

    printf("\n    EDIT QUIZ RECORD OF STUDENT #%d - %s%s%s\n\n", id, BLUE, students[id-1].name, NORMAL);
    for (m = 0; m < class_data->major_exam_count; m++)
    {
        printf("      MAJOR EXAM %d: %s", (m+1), BLUE);
        temp_data = get_int();
        printf("%s", NORMAL);

        if (temp_data < 0 || temp_data > class_data->major_exam_total[m])
        {
            printf("\n      %s[!] You entered an invalid score. Please try again.%s\n      ", RED, NORMAL);
            getch();
            return;
        }
        else
        {
            students[id-1].major_exam_scores[m] = temp_data;
        }
    }
    printf("\n      %s[!] Major Exam scores of Student #%d have been edited successfully.\n", GREEN, id);
    printf("      [!] Press any key to return...%s\n      ", NORMAL);
    getch();
}

void edit_major_exam_count_screen(struct CLASS *class_data, struct STUDENT *students)
{
    int temp_data;

    fflush(stdin);
    display_title();
    printf("  %sMENU > LOG IN > USER PANEL > MANAGE RECORD DATA > MANAGE MAJOR EXAM > EDIT MAJOR EXAM COUNT%s\n\n", RED, NORMAL);
    printf("    %sNote:%s The number of major exams must be AT LEAST 1\n\n", RED, NORMAL);
    printf("    NUMBER OF MAJOR EXAMS: %s", BLUE);
    temp_data = get_int();
    printf("%s", NORMAL);

    if (temp_data < 1)
    {
        printf("\n    %s[!] Invalid input. Please try again.%s\n    ", RED, NORMAL);
        getch();
        return;
    }
    else
        class_data->major_exam_count = temp_data;

    printf("\n    %s[!] Major Exam count has been updated successfully.\n", GREEN);
    printf("    [!] Press any key to return...%s\n    ", NORMAL);
    getch();
}

void general_student_assessment_screen(struct CLASS class_data, struct STUDENT *students)
{
    float sorted_mean_scores[class_data.size];
    float mean_scores[class_data.size];
    float mean_score_sum = 0.0;
    float class_mean;

    int rank_id[class_data.size]; // Sorted index of students
    int i;

    for (i = 0; i < class_data.size; i++)
    {
        mean_scores[i] = compute_grade(class_data, students, i);
    }

    for (i = 0; i < class_data.size; i++) // Copy mean_scores to sorted_mean_scores
    {
        sorted_mean_scores[i] = mean_scores[i];
    }

    class_mean_sort_descending(sorted_mean_scores, rank_id, class_data.size);

    for (i = 0; i < class_data.size; i++)
    {
        mean_score_sum += mean_scores[i];
    }

    class_mean = (float) mean_score_sum / class_data.size;

    fflush(stdin);
    display_title();
    printf("  %sMENU > LOG IN > USER PANEL > GENERAL STUDENT ASSESSMENT%s\n\n", RED, NORMAL);
    printf("    Class Mean: %s%.2f%%%s\n\n", BLUE, class_mean, NORMAL);
    printf("    Top 5 Students with Highest Grade:\n\n");

    for (int i = 0; i < MAX_TOP; i++)
    {
        printf("      %sTOP %d:%s   %.2f%%   %s\n", BLUE, (i+1), NORMAL, sorted_mean_scores[i], students[rank_id[i]].name); //unstable
    }

    printf("\n  Performance Analysis:\n\n");

    if (class_mean >= 0 && class_mean < 40)
    {
        printf("      The class average is categorized as %sF%s.\n\n", RED, NORMAL);
        printf("      The class may need further attention and supervision from the instructor.\n");
        printf("      It is advised for students to focus on their weakest areas and seek help\n");
        printf("      from the instructor and various learning resources.\n");
    }
    else if (class_mean >= 40 && class_mean < 50)
    {
        printf("      The class average is categorized as %sE-%s.\n\n", RED, NORMAL);
        printf("      There is still room for improvement.\n");
        printf("      It is advised for students to continue exerting academic effort and\n");
        printf("      aim for better results.\n");
    }
    else if (class_mean >= 50 && class_mean < 60)
    {
        printf("      The class average is categorized as %sE%s.\n\n", RED, NORMAL);
        printf("      The class should keep on improving their academic performance.\n");
        printf("      It is advised for students to focus on their strongest areas and challenge\n");
        printf("      themselves to achieve greater heights.\n");
    }
    else if (class_mean >= 60 && class_mean < 70)
    {
        printf("      The class average is categorized as %sD%s.\n\n", GREEN, NORMAL);
        printf("      Keep up the good work. It is advised for students to\n");
        printf("      continue pushing themselves and strive for excellence.\n");
    }
    else if (class_mean >= 70 && class_mean < 80)
    {
        printf("      The class average is categorized as %sC%s.\n\n", GREEN, NORMAL);
        printf("      Well done! Students should continue to work hard\n");
        printf("      and set new goals for themselves.\n");
    }
    else if (class_mean >= 80 && class_mean < 90)
    {
        printf("      The class average is categorized as %sB%s.\n\n", GREEN, NORMAL);
        printf("      Congratulations for an excellent academic performance! Students\n");
        printf("      should continue to work hard and set new goals for themselves.\n");
    }
    else if (class_mean >= 90 && class_mean <= 100)
    {
        printf("      The class average is categorized as %sA%s.\n\n", GREEN, NORMAL);
        printf("      Congratulations for an excellent academic performance! Students\n");
        printf("      should continue to work hard and set new goals for themselves.\n");
    }

    printf("\n    [!] Press any key to return...\n    ");
    getch(); // Wait for keypress

}

void individual_student_assessment_screen(struct CLASS class_data, struct STUDENT *students)
{
    float grade;
    float highest_quiz_rating;
    float lowest_quiz_rating;
    float highest_major_exam_rating;
    float lowest_major_exam_rating;

    int id;
    int highest_quiz;
    int lowest_quiz;
    int highest_major_exam;
    int lowest_major_exam;

    fflush(stdin);
    display_title();
    printf("  %sMENU > LOG IN > USER PANEL > INDIVIDUAL STUDENT ASSESSMENT%s\n\n", RED, NORMAL);
    display_student_list(students, class_data.size);
    printf("\n    WHICH STUDENT NUMBER TO ASSESS: %s", BLUE);

    id = get_int();
    printf("%s", NORMAL);

    if (id < 1 || id > class_data.size) // Premature exit
    {
        printf("\n    %s[!] Invalid student number. Please try again.%s\n    ", RED, NORMAL);
        getch();
        return;
    }

    grade = compute_grade(class_data, students, id-1);

    highest_quiz_rating = get_highest_rating(students[id-1].quiz_scores, class_data.quiz_total, &highest_quiz, class_data.quiz_count);
    highest_major_exam_rating = get_highest_rating(students[id-1].major_exam_scores, class_data.major_exam_total, &highest_major_exam, class_data.major_exam_count);

    lowest_quiz_rating = get_lowest_rating(students[id-1].quiz_scores, class_data.quiz_total, &lowest_quiz, class_data.quiz_count);
    lowest_major_exam_rating = get_lowest_rating(students[id-1].major_exam_scores, class_data.major_exam_total, &lowest_major_exam, class_data.major_exam_count);

    display_title();
    printf("  %sMENU > LOG IN > USER PANEL > INDIVIDUAL STUDENT ASSESSMENT%s\n\n", RED, NORMAL);
    printf("    STUDENT NAME: %s%s%s\n\n", BLUE, students[id-1].name , NORMAL );
    printf("    Quiz Rating:\n");
    printf("    Highest: %s%.2f%% (Quiz %d)%s\n", highest_quiz_rating > 50 ? GREEN : RED, highest_quiz_rating, highest_quiz+1, NORMAL);
    printf("    Lowest: %s%.2f%% (Quiz %d)%s\n\n", lowest_quiz_rating > 50 ? GREEN : RED, lowest_quiz_rating, lowest_quiz+1, NORMAL);
    printf("    Major Exam Rating:\n");
    printf("    Highest: %s%.2f%% (Major Exam %d)%s\n", highest_major_exam_rating > 50 ? GREEN : RED, highest_major_exam_rating, highest_major_exam+1, NORMAL);
    printf("    Lowest: %s%.2f%% (Major Exam %d)%s\n\n", lowest_major_exam_rating > 50 ? GREEN : RED, lowest_major_exam_rating, lowest_major_exam+1, NORMAL);
    printf("    Grade: %s%.2f%%%s\n", BLUE, grade, NORMAL);

    display_grade_categories();

    printf("    Performance Analysis:\n\n");

    if (grade >= 90 && grade <= 100)
    {
        printf("      The student's grade is categorized as %sA%s.\n\n", GREEN, NORMAL);
        printf("      Congratulations on achieving an exceptional level of performance! The student's\n");
        printf("      work consistently demonstrates a deep understanding of the course material and\n");
        printf("      showcases exceptional critical thinking skills. Their commitment to learning and\n");
        printf("      dedication to excellence are truly commendable.\n");
    }
    else if (grade >= 80 && grade <= 89)
    {
        printf("      The student's grade is categorized as %sB%s.\n\n", GREEN, NORMAL);
        printf("      Good job! The student has shown a satisfactory performance, demonstrating a good\n");
        printf("      understanding of the material. While there is room for improvement in some areas, they\n");
        printf("      should take this as an opportunity to further develop skills and enhance performance!\n");
    }
    else if (grade >= 70 && grade <= 79)
    {
        printf("      The student's grade is categorized as %sC%s.\n\n", GREEN, NORMAL);
        printf("      The student's grade has met the basic requirements of the course, but there is room for improvement.\n");
        printf("      Their understanding of the material is limited, and they may benefit from additional studying and engagement with the\n");
        printf("      course content. Consider seeking extra help or resources to enhance understanding and improve performance.\n");
    }
    else if (grade >= 60 && grade <= 69)
    {
        printf("      The student's grade is categorized as %sD%s.\n\n", GREEN, NORMAL);
        printf("      The student's grade indicates a considerably average performance. They have shown some understanding of the material,\n");
        printf("      but there are significant gaps and areas that need improvement. It is important to identify weaknesses and\n");
        printf("      take appropriate steps to address them, such as seeking additional support or revisiting the course material.\n");
    }
    else if (grade >= 50 && grade <= 59)
    {
        printf("      The student's grade is categorized as %sE%s.\n\n", GREEN, NORMAL);
        printf("      The student's grade indicates a minimum average performance. There are significant gaps in their understanding\n");
        printf("      of the course material, and their ability to apply critical thinking skills is limited. It is important\n");
        printf("      to seek support and make a concerted effort to improve performance in future quizzes or major exams, or\n");
        printf("      consider retaking the class if necessary.\n");
    }
    else if (grade < 49)
    {
        printf("      The student's grade is categorized as %sF%s.\n\n", RED, NORMAL);
        printf("      Unfortunately, the student's grade falls well below the minimum requirements for a passing grade.\n");
        printf("      They have not demonstrated a satisfactory understanding of the course material, and it is essential to\n");
        printf("      reassess their study habits and seek extensive support to make significant improvements. Consider retaking\n");
        printf("      the course or consulting with academic advisors to explore alternative paths to success.\n");
    }

    printf("\n    [!] Press any key to return...\n    ");
    getch(); // Wait for keypress
}

void remove_student_screen(struct CLASS *class_data, struct STUDENT *students)
{
    int id;

    fflush(stdin);
    display_title();
    printf("  %sMENU > LOG IN > USER PANEL > REMOVE STUDENT%s\n\n", RED, NORMAL);
    printf("    %sNote:%s Students cannot be removed once MINIMUM required number of students (%i) is reached.\n\n", RED, NORMAL, MAX_TOP);
    display_student_list(students, class_data->size);
    printf("\n    WHICH STUDENT NUMBER TO REMOVE: %s", BLUE);

    id = get_int();
    printf("%s", NORMAL);

    if (id < 1 || id > class_data->size || class_data->size <= MAX_REQ) // Premature exit
    {
        printf("\n    %s[!] Student number may be invalid or students have reached minimum required number.\n", RED);
        printf("    [!] Please try again.%s\n    ", NORMAL);
        getch();
        return;
    }

    printf("\n    %s[!] Are you sure you want to remove this student? Press [Y] if yes, otherwise any key:%s\n    ", RED, NORMAL);
    char confirm = toupper(getch());
    if (confirm != 'Y') return;

    students[id-1].is_occupied = 0;
    rearrange_array(students, class_data->size);
    class_data->size -= 1;

    printf("\n    %s[!] Student #%d has been removed successfully.\n", GREEN, id);
    printf("    [!] Press any key to return...%s\n    ", NORMAL);
    getch();
}

void add_student_screen(struct CLASS *class_data, struct STUDENT *students)
{
    int q, m;
    char new_name[] = "New Student Name";

    fflush(stdin);
    display_title();
    printf("  %sMENU > LOG IN > USER PANEL > ADD STUDENT%s\n\n", RED, NORMAL);
    printf("    %sNote:%s The MAXIMUM required number of students is One Hundred (100).\n\n", RED, NORMAL);

    if (class_data->size > MAX_STUDENT) // Premature exit
    {
        printf("\n  %s[!] Students have reached maximum required number.\n", RED);
        printf("  [!] Please try again.%s\n  ", NORMAL);
        getch();
        return;
    }

    strcpy(students[class_data->size].name, new_name);
    students[class_data->size].is_occupied = 1;
    students[class_data->size].attendance = 0;
    for (q = 0; q < class_data->quiz_count; q++)
    {
        students[class_data->size].quiz_scores[q] = 0;
    }

    for (m = 0; m < class_data->major_exam_count; m++)
    {
        students[class_data->size].major_exam_scores[m] = 0;
    }

    class_data->size += 1;

    printf("      %s[!] New student has been added successfully. You may now edit the student's information.\n", GREEN);
    printf("      [!] Press any key to return...%s\n      ", NORMAL);
    getch();
}

// Functions used for Input Validation


int is_alphanumeric(const char *str)
// Checks if the value passed is alphanumeric
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (!isalnum(str[i]))
        {
            return 0; // Not alphanumeric
        }
        i++;
    }
    return 1; // Alphanumeric
}

int is_not_excess(char *str)
{
    return (strlen(str) <= CHAR_LIMIT); // 1 - Not excess
}

int get_int(void)
{
    char number_buffer[CHAR_LIMIT];
    int n;
    int parsed_num;
    int char_count;

    fgets(number_buffer, sizeof(number_buffer), stdin); // 100
    n = sscanf(number_buffer, "%i%n", &parsed_num, &char_count); // 0
    // returns num of  successful parse/s

    return (n == 1 && char_count+1 == strlen(number_buffer) ? parsed_num : -1);
    // var = condition ? if_true : if_false
    // x > 10 ?
}

int check_account(char *filename)
// Checks if file with the filename exists
{
    char data_path[MAX_PATH];
    sprintf(data_path, "%s\\%s", full_folder_path, filename);

    FILE *file = fopen(data_path, "r");
    if (file != NULL)
    {
        fclose(file);
        return 1;   // Exists
    }
    return 0;     // Does not exist
}

int check_directory(const char* path)
// Checks if folder path exists
{
    DWORD attributes = GetFileAttributesA(path);
    return (attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY));
}

// Functions used for processing


void get_credential(char *dest)
{
    fgets(dest, CHAR_LIMIT+1, stdin);
    remove_newline(dest);
}

void get_log_in_credential(char *dest)
{
    fgets(dest, MAX_SIZE, stdin);
    remove_newline(dest);
}

int create_account(const char *encrypted_filename)
{
    char data_path[MAX_PATH];
    sprintf(data_path, "%s\\%s", full_folder_path, encrypted_filename);

    FILE *fOUT;
    fOUT = fopen(data_path,"w");

    fprintf(fOUT, "%s", "class_size: 5\nquiz_count: 1\nmajor_exam_count: 1\nattendance_percentage: 10\nquiz_percentage: 60\nmajor_exam_percentage: 30\n\nSample Student 1\n20/20\n60/60\n40/40\n\nSample Student 2\n20/20\n60/60\n40/40\n\nSample Student 3\n20/20\n60/60\n40/40\n\nSample Student 4\n20/20\n60/60\n40/40\n\nSample Student 5\n20/20\n60/60\n40/40\n");
    fclose(fOUT);
    return 1;
}

void create_data_folder(void) // tupmanila_data
{
    GetCurrentDirectoryA(MAX_PATH, current_dir);
    sprintf(full_folder_path, "%s\\%s", current_dir, FOLDER_PATH);

    if (check_directory(full_folder_path) == 0)
        CreateDirectoryA(full_folder_path, NULL);
}

void encrypt_text(const char *username, const char *password, char *dest)
{
    const char *encryption_modifier = "vSICgRPQMlnj3sb8V2ADZKxWad5ryYNUc4pOkFJXmEe06it9zoqfBuL1wT7GHh";

    int text_length = strlen(encryption_modifier);
    int username_length = strlen(username);
    int password_length = strlen(password);
    int key = 0;

    for (int i = 0; i < text_length; i++)
    {
        int key = (int)username[i % username_length] + (int)password[i % password_length];
        dest[i] = ((int)encryption_modifier[i] + key) % 26 + 'a'; // Encrypt as lowercase letters
    }
}

void remove_newline(char *str)
// Omits newline character from fgets
{
    size_t len = strlen(str);
    int i;

    for (i = 0; i < len; i++)
    {
        if (len > 0 && str[i] == '\n')
        {
            str[i] = '\0';
        }
    }
}

void load_file_data(struct CLASS *class_data, struct STUDENT *students, const char *filename)
{
    char data_path[MAX_PATH];
    int i, q, m;
    FILE *fIN;

    sprintf(data_path, "%s\\%s", full_folder_path, filename);

    fIN = fopen(data_path, "r");

    fscanf(fIN, "class_size: %d\nquiz_count: %d\nmajor_exam_count: %d\nattendance_percentage: %d\nquiz_percentage: %d\nmajor_exam_percentage: %d\n", &class_data->size, &class_data->quiz_count, &class_data->major_exam_count, &class_data->attendance_percentage, &class_data->quiz_percentage, &class_data->major_exam_percentage);

    for (i = 0; i < class_data->size; i++)
    {
        fscanf(fIN, "%[^\n]%*c", &students[i].name);
        fscanf(fIN, "%d/%d%*c", &students[i].attendance, &class_data->attendance_total);

        for (q = 0; q < class_data->quiz_count; q++)
        {
            fscanf(fIN, "%d/%d%*c", &students[i].quiz_scores[q], &class_data->quiz_total[q]);
        }

        for (m = 0; m < class_data->major_exam_count; m++)
        {
            fscanf(fIN, "%d/%d%*c", &students[i].major_exam_scores[m], &class_data->major_exam_total[m]);
        }

        fscanf(fIN, "\n"); // Skip newline to read next data

        students[i].is_occupied = 1;
    }
    fclose(fIN);
}

void save_data(struct CLASS *class_data, struct STUDENT *students, const char *filename)
{
    char data_path[MAX_PATH];
    int i, q, m;
    FILE *fOUT;

    sprintf(data_path, "%s\\%s", full_folder_path, filename);

    fOUT = fopen(data_path, "w");

    fprintf(fOUT, "class_size: %d\nquiz_count: %d\nmajor_exam_count: %d\nattendance_percentage: %d\nquiz_percentage: %d\nmajor_exam_percentage: %d", class_data->size, class_data->quiz_count, class_data->major_exam_count, class_data->attendance_percentage, class_data->quiz_percentage, class_data->major_exam_percentage);

    for (i = 0; i < class_data->size; i++)
    {
        fprintf(fOUT, "\n\n%s\n", students[i].name);
        fprintf(fOUT, "%d/%d\n", students[i].attendance, class_data->attendance_total);

        for (q = 0; q < class_data->quiz_count; q++)
        {
            fprintf(fOUT, "%d/%d ", students[i].quiz_scores[q], class_data->quiz_total[q]);
        }
        fprintf(fOUT, "\n");

        for (m = 0; m < class_data->major_exam_count; m++)
        {
            fprintf(fOUT, "%d/%d ", students[i].major_exam_scores[m], class_data->major_exam_total[m]);
        }
    }
    fclose(fOUT);
}

void rearrange_array(struct STUDENT *students, int size)
{
    int i, j;
    struct STUDENT temp;

    // Rearrange the array using bubble sort
    for (i = 0; i < size - 1; i++)
    {
        for (j = 0; j < size - i - 1; j++)
        {
            if (students[j].is_occupied == 0 && students[j + 1].is_occupied == 1)
            {
                // Swap the structures
                temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }
}

float compute_grade(struct CLASS class_data, struct STUDENT *students, int i)
{
    float grade;

    int quiz_sum = sum_array(students[i].quiz_scores, class_data.quiz_count);
    int total_quiz_sum = sum_array(class_data.quiz_total, class_data.quiz_count);
    int major_exam_sum = sum_array(students[i].major_exam_scores, class_data.major_exam_count);
    int total_major_exam_sum = sum_array(class_data.major_exam_total, class_data.major_exam_count);

    grade = (float)quiz_sum / total_quiz_sum * class_data.quiz_percentage + (float)major_exam_sum / total_major_exam_sum * class_data.major_exam_percentage + (float)students[i].attendance/class_data.attendance_total * class_data.attendance_percentage;

    return grade;
}

int sum_array(int *arr, int size)
{
    int sum = 0;

    for (int i = 0; i < size; i++)
    {
        sum += arr[i];
    }

    return sum;
}

void class_mean_sort_descending(float *arr, int *ndx, int size)
{
    int i, j, temp_index;
    float temp_data; // must be float

    for (i = 0; i < size; i++)
    {
        ndx[i] = i;
    }

    for (i = 0; i < size - 1; i++)
    {
        for (j = 0; j < size - i - 1; j++)
        {
            if (arr[j] < arr[j + 1])
            {
                // Swap arr[j] and arr[j + 1]
                temp_data = arr[j];
                temp_index = ndx[j];

                arr[j] = arr[j + 1];
                ndx[j] = ndx[j+1];

                arr[j + 1] = temp_data;
                ndx[j+1] = temp_index;
            }
        }
    }
}

float get_highest_rating(int *scores, int *total_scores ,int *item, int size)
{
    float ratings[size];
    float highest_rating;
    int i;

    for (i = 0; i < size; i++)
    {
        ratings[i] = (float)scores[i] / total_scores[i];
        ratings[i] *= 100;
    }

    highest_rating = ratings[0]; // Assume the first element is the highest score

    *item = 0;

    for (i = 1; i < size; i++)
    {
        if (ratings[i] > highest_rating)
        {
            highest_rating = ratings[i];
            *item = i;
        }
    }
    return highest_rating;
}

float get_lowest_rating(int *scores, int *total_scores ,int *item, int size)
{
    float ratings[size];
    float lowest_rating;
    int i;

    for (i = 0; i < size; i++)
    {
        ratings[i] = (float)scores[i] / total_scores[i];
        ratings[i] *= 100;
    }

    lowest_rating = ratings[0]; // Assume the first element is the highest score

    *item = 0;

    for (i = 1; i < size; i++)
    {
        if (ratings[i] < lowest_rating)
        {
            lowest_rating = ratings[i];
            *item = i;
        }
    }

    return lowest_rating;
}

// Display Functions


void display_title(void)
{
    system("cls");
    printf("\n  %sSubmitted by Team C | Midterm Project | Computer Programming 2 | BSIS - 1AB%s\n", BLUE, NORMAL);
    printf("  -------------------------------------------------------------------------------------\n");
    printf("     Intelligent Student Record Management System and Assessment Tool for TUP Manila   \n");
    printf("  -------------------------------------------------------------------------------------\n");
}

void display_individual_student_data(struct CLASS class_data, struct STUDENT *students)
{
    float grade;
    int i, q, m;

    display_title();
    printf("  %sMENU > LOG IN > USER PANEL > STUDENT RECORD > DISPLAY INDIVIDUAL STUDENT DATA%s\n", RED, NORMAL);

    for (i = 0; i < class_data.size; i++)
    {
        grade = compute_grade(class_data, students, i);
        printf("    ---------------------------------\n");
        printf("    %s%s STUDENT NO. %d                   %s%s\n\n", WHITE, BLACKBG, (i+1), WHITEBG, NORMAL);
        printf("      Name: %s%s%s\n", BLUE, students[i].name, NORMAL);
        printf("      Grade: %s%.2f%s\n", BLUE, grade, NORMAL);
        printf("      Attendance: %s%d/%d%s\n\n", BLUE, students[i].attendance, class_data.attendance_total, NORMAL);

        for (q = 0; q < class_data.quiz_count; q++)
        {
            printf("      Quiz #%d: %s%d/%d%s\n", (q+1), BLUE, students[i].quiz_scores[q], class_data.quiz_total[q], NORMAL);
        }
        printf("\n");
        for (m = 0; m < class_data.major_exam_count; m++)
        {
            printf("      Major Exam #%d: %s%d/%d%s\n", (m+1), BLUE, students[i].major_exam_scores[m], class_data.major_exam_total[m], NORMAL);
        }
        printf("    ---------------------------------\n");
    }
    printf("\n    [!] Press any key to return...\n    ");
    getch();
}

void display_student_list(struct STUDENT *students, int size)
{
    int s;

    printf("    STUDENT LIST:\n\n");

    for (s = 0; s < size; s++)
    {
        printf("    %s%d%s - %s\n", BLUE, (s+1), NORMAL, students[s].name);
    }
}

void display_grade_categories(void)
{
    printf("\n    Grade Categories:\n\n");
    printf("      %s90-100 : %s A\n", BLUE, NORMAL);
    printf("      %s80-89  : %s B\n", BLUE, NORMAL);
    printf("      %s70-79  : %s C\n", BLUE, NORMAL);
    printf("      %s60-69  : %s D\n", BLUE, NORMAL);
    printf("      %s50-59  : %s E\n", BLUE, NORMAL);
    printf("      %s 0-49  : %s F\n\n", BLUE, NORMAL);
}

