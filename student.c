#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define TOTAL_WEEKS 24

typedef struct {
    int id;
    char name[100];
    char attend[TOTAL_WEEKS + 1];
    char hw[TOTAL_WEEKS + 1];
    float progress;
    char status[20];
} Student;

void clear_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void calculate_metrics(Student *s)
 {
    int i;
    int pts = 0;
    for (i = 0; i < TOTAL_WEEKS; i++)
     {
        if (s->attend[i] == 'P') pts++;
        if (s->hw[i] == 'S') pts++;
    }
    s->progress = ((float)pts / (TOTAL_WEEKS * 2)) * 100.0f;

    if (s->progress >= 90.0f)
     strcpy(s->status, "Excellent");
    else if (s->progress >= 70.0f)
     strcpy(s->status, "On Track");
    else if (s->progress >= 50.0f)
     strcpy(s->status, "Needs Attention");
    else strcpy(s->status, "Critical");
}

void load_data(Student s[], int *count)
 {
    FILE *f = fopen("database.txt", "r");
    if (!f) return;
    char line[512];
    while (*count < MAX_STUDENTS && fgets(line, sizeof(line), f))
     {
        int id;
        char name[100], att[TOTAL_WEEKS + 1], hw[TOTAL_WEEKS + 1];
        if (sscanf(line, "%d|%99[^|]|%24s|%24s", &id, name, att, hw) == 4)
         {
            s[*count].id = id;
            snprintf(s[*count].name, sizeof(s[*count].name), "%s", name);
            snprintf(s[*count].attend, sizeof(s[*count].attend), "%s", att);
            snprintf(s[*count].hw, sizeof(s[*count].hw), "%s", hw);
            calculate_metrics(&s[*count]);
            (*count)++;
        }
    }
    fclose(f);
}

void save_data(Student s[], int count)
 {
    int i;
    FILE *f = fopen("database.txt", "w");
    if (!f) {
        printf("Error opening database for writing.\n");
        return;
    }
    for (i = 0; i < count; i++)
     {
        fprintf(f, "%d|%s|%s|%s\n", s[i].id, s[i].name, s[i].attend, s[i].hw);
    }
    fclose(f);
    printf("Data saved successfully to database.txt!\n");
}

void view_dashboard(Student s[], int count)
 {
    int i, w;
    int exc = 0, on_tr = 0, needs_att = 0, crit = 0;
    float total_prog = 0.0f;

    for (i = 0; i < count; i++)
     {
        calculate_metrics(&s[i]);
        total_prog += s[i].progress;
        if (strcmp(s[i].status, "Excellent") == 0) exc++;
        else if (strcmp(s[i].status, "On Track") == 0) on_tr++;
        else if (strcmp(s[i].status, "Needs Attention") == 0) needs_att++;
        else crit++;
    }

    float class_avg = (count > 0) ? (total_prog / count) : 0.0f;

    printf("\n=========================================================\n");
    printf("          STUDENT TRACKING DASHBOARD\n");
    printf("=========================================================\n");
    printf("  Total Students:             %d\n", count);
    printf("  Class Average Progress:     %.2f%%\n", class_avg);
    printf("---------------------------------------------------------\n");
    printf("  [+] Excellent (>=90%%):        %d\n", exc);
    printf("  [+] On Track (70-89%%):        %d\n", on_tr);
    printf("  [+] Needs Attention (50-69%%): %d\n", needs_att);
    printf("  [+] Critical (<50%%):          %d\n", crit);
    printf("=========================================================\n");

    printf("\n--- WEEKLY  OVERVIEW ---\n");
    printf("%-7s | %-10s | %-8s | %-8s | %-12s\n", "Week", "Completed", "Partial", "Pending", "Rate %");
    printf("---------------------------------------------------------\n");
    
    for (w = 0; w < TOTAL_WEEKS; w++)
     {
        int comp = 0, part = 0, pend = 0;
        for (i = 0; i < count; i++) {
            int p = (s[i].attend[w] == 'P');
            int h = (s[i].hw[w] == 'S');
            if (p && h) comp++;
            else if (p || h) part++;
            else pend++;
        }
        float rate = (count > 0) ? ((float)comp / count) * 100.0f : 0.0f;
        printf("W-%-4d | %-10d | %-8d | %-8d | %.1f%%\n", w + 1, comp, part, pend, rate);
    }
}

void view_roster(Student s[], int count) {
    int i, w;
    if (count == 0) {
        printf("No student records found.\n");
        return;
    }

    printf("\n--- STUDENT DATA & PERFORMANCE ---\n");
    printf("%-4s | %-3s | %-16s | %-9s | %-16s | W1 W2 W3\n", "S.N.", "ID", "Student Name", "Progress", "Overall Status");
    printf("--------------------------------------------------------------------\n");

    for (i = 0; i < count; i++)
     {
        calculate_metrics(&s[i]);
        printf("%-4d | %-3d | %-16s | %-7.1f%% | %-16s | ", i + 1, s[i].id, s[i].name, s[i].progress, s[i].status);
        
        for (w = 0; w < 3; w++)
         {
            char code = '-';
            if (s[i].attend[w] == 'P' && s[i].hw[w] == 'S')
             code = 'C';
            else if (s[i].attend[w] == 'P' || s[i].hw[w] == 'S') 
            code = 'P';
            printf("%c  ", code);
        }
        printf("\n");
    }
}

void add_student(Student s[], int *count)
 {
    int i;
    if (*count >= MAX_STUDENTS)
     {
        printf("Cannot add more.\n");
        return;
    }
    clear_buffer();
    printf("Enter Student Name: ");
    char name[100];
    if (scanf("%99[^\n]", name) != 1)
     {
        printf("Failed to read name.\n");
        return;
    }

    s[*count].id = *count + 1;
    snprintf(s[*count].name, sizeof(s[*count].name), "%s", name);
    for (i = 0; i < TOTAL_WEEKS; i++)
     {
        s[*count].attend[i] = '-';
        s[*count].hw[i] = '-';
    }
    s[*count].attend[TOTAL_WEEKS] = '\0';
    s[*count].hw[TOTAL_WEEKS] = '\0';
    calculate_metrics(&s[*count]);
    (*count)++;
    printf("Student added successfully!\n");
}

void update_student(Student s[], int count)
 {
    int i;
    int id, week;
    printf("Enter Student ID: ");
    if (scanf("%d", &id) != 1)
     {
        clear_buffer();
        printf("Invalid ID format.\n");
        return;
    }

    int idx = -1;
    for (i = 0; i < count; i++)
     {
        if (s[i].id == id) {
            idx = i;
            break;
        }
    }

    if (idx == -1)
     {
        printf("Student ID not found.\n");
        return;
    }

    printf("Enter Week Number (1-%d): ", TOTAL_WEEKS);
    if (scanf("%d", &week) != 1 || week < 1 || week > TOTAL_WEEKS)
     {
        clear_buffer();
        printf("Invalid week range.\n");
        return;
    }

    char att, hw;
    printf("Attendance [P/A]: ");
    scanf(" %c", &att);
    printf("Homework [S/N]: ");
    scanf(" %c", &hw);

    att = (att >= 'a' && att <= 'z') ? att - 'a' + 'A' : att;
    hw = (hw >= 'a' && hw <= 'z') ? hw - 'a' + 'A' : hw;

    if ((att != 'P' && att != 'A') || (hw != 'S' && hw != 'N'))
     {
        printf("Invalid input. Use P or A for attendance, S or N for homework.\n");
        return;
    }

    s[idx].attend[week - 1] = att;
    s[idx].hw[week - 1] = hw;
    calculate_metrics(&s[idx]);
    printf("Student record updated.\n");
}

int main(void) {
    Student s[MAX_STUDENTS];
    int count = 0;
    char pass[20];
    int choice;

    printf("Password (default: teacher): ");
    if (scanf("%19s", pass) != 1 || strcmp(pass, "teacher") != 0)
     {
        printf("Password incorrect.\n");
        return 0;
    }
    clear_buffer();
    load_data(s, &count);

    do {
        printf("\n=========================================================\n");
        printf("\n--- SWSMT-BIT  Portal ---\n");
        printf("\n=========================================================\n");
        printf("1. View Dashboard\n");
        printf("\n=========================================================\n");
        printf("2. Student Table\n");
        printf("\n=========================================================\n");
        printf("3. Add New Student\n");
        printf("\n=========================================================\n");
        printf("4. Attendance & Homework\n");
        printf("\n=========================================================\n");
        printf("5. Save\n");
        printf("\n=========================================================\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1)
         {
            clear_buffer();
            choice = 0;
            continue;
        }

        if (choice == 1)
         view_dashboard(s, count);
        else if
         (choice == 2) 
         view_roster(s, count);
        else if 
        (choice == 3)
         add_student(s, &count);
        else if 
        (choice == 4)
        update_student(s, count);
        else if 
        (choice == 5)
         {
            save_data(s, count);
            break;
        }
        else printf("Invalid selection option.\n");
    } while (choice != 5);

    return 0;
}