#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_STU 100
#define MAX_SUB 5
#define NAME_LENGTH 50
#define SUB_LEN 30
#define L_LEN 512

int readLine(FILE *fp, char line[])
{
    if (fgets(line, L_LEN, fp) == NULL)
        return 0;

    int len = strlen(line);

    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
    }
    else if (len == L_LEN - 1) {
        int ch;
        while ((ch = fgetc(fp)) != '\n' && ch != EOF);
        return -1;
    }

    return 1;
}

int sameline(char a[], char b[])
{
    if (strlen(a) != strlen(b))
        return 0;

    for (int i = 0; a[i] != '\0'; i++) {
        if (tolower((unsigned char)a[i]) !=
            tolower((unsigned char)b[i]))
            return 0;
    }

    return 1;
}

int validId(char id[])
{
    if (strlen(id) != 8 || id[3] != '-')
        return 0;

    if (id[2] != 'K' && id[2] != 'k')
        return 0;

    for (int i = 0; i < 8; i++) {
        if (i != 2 && i != 3 &&
            (id[i] < '0' || id[i] > '9'))
            return 0;
    }

    return 1;
}

int hastext(char text[])
{
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] != ' ' && text[i] != '\t')
            return 1;
    }

    return 0;
}

int validSubject(char sub[])
{
    if (strlen(sub) >= SUB_LEN)
        return 0;

    if (!hastext(sub))
        return 0;

    if (strchr(sub, '|') != NULL ||
        strchr(sub, ',') != NULL ||
        strchr(sub, '\n') != NULL)
        return 0;

    return 1;
}

int readSubjects(char text[], char subjects[][SUB_LEN])
{
    int count = 0;
    int i = 0;

    while (text[i] != '\0') {

        if (count >= MAX_SUB)
            return -1;

        int j = 0;

        while (text[i] != ',' && text[i] != '\0') {

            if (j >= SUB_LEN - 1)
                return -1;

            subjects[count][j] = text[i];
            j++;
            i++;
        }

        subjects[count][j] = '\0';

        if (!validSubject(subjects[count]))
            return -1;

        count++;

        if (text[i] == ',') {
            i++;

            if (text[i] == '\0')
                return -1;
        }
    }

    return count;
}

int readStudent(char line[], int pos, char name[][NAME_LENGTH],
                char id[][15], char subs[][MAX_SUB][SUB_LEN],
                int subcount[], char learn[][MAX_SUB][SUB_LEN],
                int learncount[])
{
    char data[6][L_LEN];
    int i = 0;

    for (int f = 0; f < 6; f++) {
        int j = 0;

        while (line[i] != '|' && line[i] != '\0') {

            if (j >= L_LEN - 1)
                return 0;

            data[f][j] = line[i];
            j++;
            i++;
        }

        data[f][j] = '\0';

        if (line[i] == '|') {
            if (f == 5)
                return 0;

            i++;
        }
        else if (f < 5) {
            return 0;
        }
    }

    if (!validId(data[0]))
        return 0;

    if (strlen(data[1]) >= NAME_LENGTH ||
        !hastext(data[1]) ||
        strchr(data[1], '\r') != NULL)
        return 0;

    int teach = readSubjects(data[3], subs[pos]);
    int help = readSubjects(data[5], learn[pos]);

    if (teach < 0 || help < 0)
        return 0;

    if (strlen(data[2]) != 1 ||
        data[2][0] - '0' != teach)
        return 0;

    if (strlen(data[4]) != 1 ||
        data[4][0] - '0' != help)
        return 0;

    strcpy(id[pos], data[0]);
    strcpy(name[pos], data[1]);

    subcount[pos] = teach;
    learncount[pos] = help;

    return 1;
}

int alrdyExist(char id[][15], int count, char findId[])
{
    for (int i = 0; i < count; i++) {
        if (sameline(id[i], findId))
            return 1;
    }

    return 0;
}

int loadStudents(char name[][NAME_LENGTH], char id[][15],
                 char subs[][MAX_SUB][SUB_LEN], int subcount[],
                 char learn[][MAX_SUB][SUB_LEN], int learncount[])
{
    FILE *fp = fopen("students.txt", "r");

    if (fp == NULL) {

        char first[] =
        "26K-0001|Taha Mayet|2|Calculus,C Programming|1|Functional English";

        char second[] =
        "26K-0002|Ahmed Kazmi|1|Functional English|2|calculus,C Programming";

        if (!readStudent(first, 0, name, id, subs, subcount,
                         learn, learncount) ||
            !readStudent(second, 1, name, id, subs, subcount,
                         learn, learncount))
            return -1;

        printf("Using sample students. Could not open students.txt.\n");

        return 2;
    }

    char line[L_LEN];
    int count = 0;
    int lineNo = 0;

    while (1) {

        int result = readLine(fp, line);

        if (result == 0)
            break;

        lineNo++;

        if (result == 1 && line[0] == '\0')
            continue;

        if (result < 0 || count >= MAX_STU ||
            !readStudent(line, count, name, id, subs,
                         subcount, learn, learncount)) {

            printf("Invalid record or too many students at line %d.\n",
                   lineNo);

            count = -1;
            break;
        }

        if (alrdyExist(id, count, id[count])) {

            printf("Duplicate student ID on line %d.\n", lineNo);

            count = -1;
            break;
        }

        count++;
    }

    if (ferror(fp)) {
        puts("Could not read students.txt.");
        count = -1;
    }

    fclose(fp);

    return count;
}

void printSubjects(FILE *fp, char subjects[][SUB_LEN], int count)
{
    if (count <= 0)
        return;

    fprintf(fp, "%s", subjects[0]);

    for (int i = 1; i < count; i++) {
        fprintf(fp, ",%s", subjects[i]);
    }
}

int save(int count, char name[][NAME_LENGTH], char id[][15],
         char subs[][MAX_SUB][SUB_LEN], int subcount[],
         char learn[][MAX_SUB][SUB_LEN], int learncount[])
{
    char tempFile[] = "students.tmp";
    char fileName[] = "students.txt";

    FILE *fp = fopen(tempFile, "w");

    if (fp == NULL) {
        puts("Could not save students.txt.");
        return 0;
    }

    for (int i = 0; i < count; i++) {

        fprintf(fp, "%s|%s|%d|",
                id[i], name[i], subcount[i]);

        printSubjects(fp, subs[i], subcount[i]);

        fprintf(fp, "|%d|", learncount[i]);

        printSubjects(fp, learn[i], learncount[i]);

        fputc('\n', fp);
    }

    int error = ferror(fp);

    if (fclose(fp) != 0)
        error = 1;

    if (error || rename(tempFile, fileName) != 0) {

        remove(tempFile);

        puts("Could not save students.txt.");

        return 0;
    }

    printf("Saved %d student record(s) to %s.\n",
           count, fileName);

    return 1;
}

void Liststudents(int count, char name[][NAME_LENGTH],
                  char id[][15], char subs[][MAX_SUB][SUB_LEN],
                  int subcount[], char learn[][MAX_SUB][SUB_LEN],
                  int learncount[])
{
    printf("\nTotal students: %d\n", count);

    for (int i = 0; i < count; i++) {

        printf("\n%s - %s\n", id[i], name[i]);

        printf("  Can teach: ");

        if (subcount[i] == 0)
            printf("None");

        printSubjects(stdout, subs[i], subcount[i]);

        printf("\n  Needs help: ");

        if (learncount[i] == 0)
            printf("None");

        printSubjects(stdout, learn[i], learncount[i]);

        printf("\n\n");
    }
}

int hasSubject(char subjects[][SUB_LEN], int count, char sub[])
{
    for (int i = 0; i < count; i++) {

        if (sameline(subjects[i], sub))
            return 1;
    }

    return 0;
}

void showSubjectReport(int count, char subs[][MAX_SUB][SUB_LEN],
                       int subcount[], char learn[][MAX_SUB][SUB_LEN],
                       int learncount[])
{
    char sub[L_LEN];

    int teach = 0;
    int help = 0;

    printf("Enter a subject: \n");

    if (readLine(stdin, sub) != 1 || !validSubject(sub)) {
        puts("Enter a valid subject name.");
        return;
    }

    for (int i = 0; i < count; i++) {

        if (hasSubject(subs[i], subcount[i], sub))
            teach++;

        if (hasSubject(learn[i], learncount[i], sub))
            help++;
    }

    printf("\nTotal students: %d\n", count);

    printf("Can teach %s: %d\n", sub, teach);

    printf("Needs help with %s: %d\n", sub, help);
}

int main(void)
{
    char name[MAX_STU][NAME_LENGTH];
    char id[MAX_STU][15];

    char subs[MAX_STU][MAX_SUB][SUB_LEN];
    int subcount[MAX_STU];

    char learn[MAX_STU][MAX_SUB][SUB_LEN];
    int learncount[MAX_STU];

    char choice[L_LEN];

    int count;
    int result;
    int running = 1;
    int exitStatus = 0;

    count = loadStudents(name, id, subs, subcount,
                         learn, learncount);

    if (count < 0) {
        exitStatus = 1;
        running = 0;
    }
    else {
        printf("Loaded %d student record.\n", count);
    }

    while (running == 1) {

        printf("\n\n   STUDENT HELP   \n\n");

        printf("1. List all students\n");
        printf("2. Subject report\n");
        printf("3. Save students\n");
        printf("0. Exit\n");

        printf("Choose an option: ");

        result = readLine(stdin, choice);

        if (result == 0) {
            running = 0;
        }
        else if (result < 0 || strlen(choice) != 1) {
            printf("Invalid choice. Enter 0, 1, 2, or 3.\n");
        }
        else if (choice[0] == '1') {

            Liststudents(count, name, id, subs, subcount,
                         learn, learncount);
        }
        else if (choice[0] == '2') {

            showSubjectReport(count, subs, subcount,
                              learn, learncount);
        }
        else if (choice[0] == '3') {

            save(count, name, id, subs, subcount,
                 learn, learncount);
        }
        else if (choice[0] == '0') {
            running = 0;
        }
        else {
            printf("Invalid choice. Enter 0, 1, 2, or 3.\n");
        }
    }

    return exitStatus;
}
