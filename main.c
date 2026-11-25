#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define max_std 100
#define max_sub 5
#define max_name 50
#define max_sub_len 30

// Muhammad Ali's part  26K-0548
typedef struct {
    int id;
    char name[max_name];

    char teach[max_sub][max_sub_len];
    int teachCount;

    char need[max_sub][max_sub_len];
    int needCount;
} Student;

Student students[max_std];
int studentCount = 0;

void read_line(char s[], int size, FILE *fp){
    if (fgets(s, size, fp) != NULL)
        s[strcspn(s, "\n")] = '\0';
}

void read_subjects(char subjects[][max_sub_len], int *count){
    printf("How many subjects? (0-5): ");
    scanf("%d", count);
    getchar();

    for (int i = 0; i < *count; i++) {
        printf("Enter subject %d: ", i + 1);
        read_line(subjects[i], max_sub_len, stdin);
    }
}

int find_student(int id){
    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id)
            return i;
    }
    return -1;
}

void display_student(int index){
    printf("\nID: %d\n", students[index].id);
    printf("Name: %s\n", students[index].name);
    printf("Can teach:\n");
    for (int i = 0; i < students[index].teachCount; i++) {
        printf("%s\n", students[index].teach[i]);
    }

    printf("Needs help in:\n");
    for (int i = 0; i < students[index].needCount; i++) {
        printf("%s\n", students[index].need[i]);
    }
    printf("\n");
}

void register_student(){
    if (studentCount == max_std) {
        printf("Student list is full!\n");
        return;
    }

    int id;
    printf("\nEnter student ID: ");
    scanf("%d", &id);
    getchar();
    if (find_student(id) != -1) {
        printf("ID already exists!\n");
        return;
    }
    students[studentCount].id = id;
    printf("Enter student name: ");
    read_line(students[studentCount].name, max_name, stdin);
    printf("\nSubjects the student can teach:\n");
    read_subjects(students[studentCount].teach, &students[studentCount].teachCount);

    printf("\nSubjects the student needs help in:\n");
    read_subjects(students[studentCount].need, &students[studentCount].needCount);

    studentCount++;
    printf("\nStudent registered successfully!\n");
}

void update_profile()
{
    int id, index, choice;
    printf("\nEnter student ID to update: ");
    scanf("%d", &id);
    getchar();
    index = find_student(id);
    if (index == -1) {
        printf("Student not found!\n");
        return;
    }
    do {
        printf("\n1. Change name\n");
        printf("2. Change teaching subjects\n");
        printf("3. Change needed subjects\n");
        printf("4. Finish\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("Enter new name: ");
                read_line(students[index].name, max_name, stdin);
                break;
            case 2:
                read_subjects(students[index].teach, &students[index].teachCount);
                break;
            case 3:
                read_subjects(students[index].need, &students[index].needCount);
                break;
            case 4:
                printf("Finished updating.\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 4);}

void remove_profile(){
    int id, index;
    printf("\nEnter student ID to remove: ");
    scanf("%d", &id);
    getchar();
    index = find_student(id);
    if (index == -1) {
        printf("Student not found!\n");
        return;
    }
    for (int i = index; i < studentCount - 1; i++) {
        students[i] = students[i + 1];
    }
    studentCount--;
    printf("Student removed successfully!\n");
}

void display_all(){
    if (studentCount == 0) {
        printf("No students registered.\n");
        return;
    }
    for (int i = 0; i < studentCount; i++) {
        printf("\nStudent %d\n", i + 1);
        display_student(i);
    }}

void save_students(){
    FILE *fp;
    int i, j;
    fp = fopen("Students.txt", "w");
    if (fp == NULL) {
        printf("Could not save to Students.txt\n");
        return;
    }
    for (i = 0; i < studentCount; i++) {
        fprintf(fp, "%d\n", students[i].id);
        fprintf(fp, "%s\n", students[i].name);

        fprintf(fp, "%d\n", students[i].teachCount);
        for (j = 0; j < students[i].teachCount; j++) {
            fprintf(fp, "%s\n", students[i].teach[j]);
        }
        fprintf(fp, "%d\n", students[i].needCount);
        for (j = 0; j < students[i].needCount; j++) {
            fprintf(fp, "%s\n", students[i].need[j]);
        }
    }
    fclose(fp);
}


// Ahmad Raza's part  26K-0609
int is_same(const char a[], const char b[]){
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (tolower((unsigned char)a[i]) != tolower((unsigned char)b[i]))
            return 0;
        i++;
    }
    return a[i] == b[i];
}
int check_teach(const Student *s, const char subject[]){
    int i;
    for (i = 0; i < s->teachCount; i++) {
        if (is_same(s->teach[i], subject))
            return 1;
    }
    return 0;}

int count_help(const Student *teacher, const Student *need){
    int i, covered = 0;
    for (i = 0; i < need->needCount; i++) {
        if (check_teach(teacher, need->need[i]))
            covered++;
    }
    return covered;}

void search_teacher(Student list[], int count, char subject[]){
    int i, found = 0;

    printf("\n--- Students who can teach \"%s\" ---\n", subject);
    for (i = 0; i < count; i++) {
        if (check_teach(&list[i], subject)) {
            printf("  %s (%d)\n", list[i].name, list[i].id);
            found++;
        }
    }
    if (found == 0)
        printf("  No student teaches this subject yet.\n");
    else
        printf("Total found: %d\n", found);
}

void show_match(Student list[], int count, int index){
    int i, j, covered, fullFound = 0, partialFound = 0;
    Student *me;

    if (index < 0 || index >= count) {
        printf("Invalid student.\n");
        return;
    }
    me = &list[index];

    if (me->needCount == 0) {
        printf("%s has not listed any subjects they need help with.\n", me->name);
        return;
    }

    printf("\n--- Matches for %s (%d) ---\n", me->name, me->id);

    printf("Can teach everything you need:\n");
    for (i = 0; i < count; i++) {
        if (i == index)
            continue;
        if (count_help(&list[i], me) == me->needCount) {
            printf("  %s (%d)\n", list[i].name, list[i].id);
            fullFound++;
        }
    }
    if (fullFound == 0)
        printf("  None.\n");

    printf("Can teach part of what you need:\n");
    for (i = 0; i < count; i++) {
        if (i == index)
            continue;
        covered = count_help(&list[i], me);
        if (covered > 0 && covered < me->needCount) {
            printf("  %s (%d) - covers %d of %d: ",
                   list[i].name, list[i].id, covered, me->needCount);
            for (j = 0; j < me->needCount; j++) {
                if (check_teach(&list[i], me->need[j]))
                    printf("%s ", me->need[j]);
            }
            printf("\n");
            partialFound++;
        }
    }
    if (partialFound == 0)
        printf("  None.\n");
}
void show_partners(Student list[], int count){
    int a, b, i, found = 0;

    printf("\n--- Mutual Study Partners ---\n");
    for (a = 0; a < count; a++) {
        for (b = a + 1; b < count; b++) {

            int aToB = count_help(&list[a], &list[b]);
            int bToA = count_help(&list[b], &list[a]);

            if (aToB > 0 && bToA > 0) {
                found++;
                printf("%d) %s (%d)  <->  %s (%d)\n", found,
                       list[a].name, list[a].id, list[b].name, list[b].id);

                printf("     %s teaches %s: ", list[a].name, list[b].name);
                for (i = 0; i < list[b].needCount; i++) {
                    if (check_teach(&list[a], list[b].need[i]))
                        printf("%s ", list[b].need[i]);
                }
                printf("\n");
                printf("     %s teaches %s: ", list[b].name, list[a].name);
                for (i = 0; i < list[a].needCount; i++) {
                    if (check_teach(&list[b], list[a].need[i]))
                        printf("%s ", list[a].need[i]);
                }
                printf("\n");}
        }
    }
    if (found == 0)
        printf("  No mutual study partners found.\n");
}

// Taha Hamza's part  26K-0566
void load_students(){
    FILE *fp = fopen("Students.txt", "r");
    if (fp == NULL)
        return;

    while (studentCount < max_std && fscanf(fp, "%d", &students[studentCount].id) == 1) {
        Student *s = &students[studentCount];
        int i;

        fgetc(fp);
        read_line(s->name, max_name, fp);

        fscanf(fp, "%d", &s->teachCount);
        fgetc(fp);
        for (i = 0; i < s->teachCount; i++)
            read_line(s->teach[i], max_sub_len, fp);

        fscanf(fp, "%d", &s->needCount);
        fgetc(fp);
        for (i = 0; i < s->needCount; i++)
            read_line(s->need[i], max_sub_len, fp);

        studentCount++;
    }
    fclose(fp);
    printf("Loaded %d student record(s).\n", studentCount);
}
void show_subject_report(){
    char sub[max_sub_len];
    int teach = 0, help = 0;

    printf("Enter a subject: ");
    read_line(sub, max_sub_len, stdin);

    for (int i = 0; i < studentCount; i++) {
        if (check_teach(&students[i], sub))
            teach++;

        for (int j = 0; j < students[i].needCount; j++) {
            if (is_same(students[i].need[j], sub)) {
                help++;
                break;}
        }
    }

    printf("\nTotal students: %d\n", studentCount);
    printf("Can teach %s: %d\n", sub, teach);
    printf("Needs help with %s: %d\n", sub, help);
}

int main(){
    int choice, id;
    char subject[max_sub_len];

    load_students();

    do {
        printf("\n===== StudentHelp =====\n");
        printf("1. Register student\n");
        printf("2. Update profile\n");
        printf("3. Remove profile\n");
        printf("4. Display all students\n");
        printf("5. Search teacher by subject\n");
        printf("6. Show matches for a student\n");
        printf("7. Show mutual study partners\n");
        printf("8. Subject report\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                register_student();
                break;
            case 2:
                update_profile();
                break;
            case 3:
                remove_profile();
                break;
            case 4:
                display_all();
                break;
            case 5:
                printf("Enter subject: ");
                read_line(subject, max_sub_len, stdin);
                search_teacher(students, studentCount, subject);
                break;
            case 6:
                printf("Enter student ID: ");
                scanf("%d", &id);
                getchar();
                show_match(students, studentCount, find_student(id));
                break;
            case 7:
                show_partners(students, studentCount);
                break;
            case 8:
                show_subject_report();
                break;
            case 0:
                printf("Byee!\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 0);

    save_students();
    return 0;
}

