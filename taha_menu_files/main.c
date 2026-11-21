#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_STU 100
#define MAX_SUB 5
#define NAME_LENGTH 50
#define SUB_LEN 30
#define L_LEN 512

int readLine(FILE *file, char line[]) //returnS, 0,1 OR -1
{
    if (fgets(line, L_LEN, file) == NULL)
        return 0;

    int length = strlen(line);
    if (length == L_LEN - 1 && line[length - 1] != '\n') {
        int ch = fgetc(file);
        int chcount= 0;
        while (ch != '\n' && ch != EOF) {
        ch = fgetc(file);
        chcount+=1;
        }
        return -1,chcount;
    }

    if (length > 0 && line[length - 1] == '\n')
        length--;
    line[length] = 0;
    return 1;
}

int sameline(char first[], char second[])
{
    if (strlen(first) != strlen(second))
        return 0;

    for (int i = 0; first[i] != '\0'; i++) {
        char a = first[i];
        char b = second[i];
        if (tolower(a) != tolower(b))
            return 0;
    }
    return 1;
}
//eg.26K-0566, lwk extra
int validId(char id[])
{
    if (strlen(id) != 8 || id[3] != '-')
        return 0;
    if (id[2] != 'K' && id[2] != 'k')
        return 0;

    for (int i = 0; i < 8; i++) {
        if (i != 2 && i != 3 && (id[i] < '0' || id[i] > '9'))
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

int validSubject(char subject[])
{
    if (strlen(subject) >= SUB_LEN)
        return 0;

    if (hastext(subject) == 0)
        return 0;

    if (strchr(subject, '|') != NULL)
        return 0;

    if (strchr(subject, ',') != NULL)
        return 0;

    if (strchr(subject, '\n') != NULL)
        return 0;

    return 1;
}

int readSubjects(char subs[], char subjects[][SUB_LEN])
{
    int count = 0, i=0;

    while (subs[i] != '\0') {
        if (count >= MAX_SUB)
            return -1;

        int letter = 0;
        while (subs[i] != ',' && subs[i] != '\0') {
            if (letter == SUB_LEN - 1)
                return -1;
            subjects[count][letter] = subs[i];
            i++;
            letter++;
        }

        subjects[count][letter] = '\0';
        if (!validSubject(subjects[count]))
            return -1;
        count++;

        if (subs[i] == ',') {
            i++;
            if (subs[i] == '\0')
                return -1;
        }
    }
    return count;
}
// could use struct...
int readStudent(char line[], int index, char name[][NAME_LENGTH], char id[][15],
                char subs[][MAX_SUB][SUB_LEN], int subcount[],
                char learn[][MAX_SUB][SUB_LEN], int learncount[])
{
    char fields[6][L_LEN];
    int i = 0;

    for(int field = 0; field < 6; field++) {
        int column = 0;
        while(line[i] != '|' && line[i] != '\0') {
            fields[field][column] = line[i];
            i++;
            column++;
        }
        fields[field][column] = '\0';

        if (line[i] == '|') {
            if(field == 5)
                return 0;
            i++;
        } else if(field < 5) {
            return 0;
        }
    }
//checks:
    if(!validId(fields[0]))
    return 0;
    if(strlen(fields[1]) >= NAME_LENGTH || !hastext(fields[1]) || strchr(fields[1], '\r') != NULL)
        return 0;

    int subjectsToTeach = readSubjects(fields[3], subs[index]);
    int subjectsToLearn = readSubjects(fields[5], learn[index]);
    if (subjectsToTeach < 0 || subjectsToLearn < 0)
        return 0;
    //using acii ('0'=48)
    if(strlen(fields[2]) != 1 || fields[2][0] - '0' != subjectsToTeach)
        return 0;
    if(strlen(fields[4]) != 1 || fields[4][0] - '0' != subjectsToLearn)
        return 0;

strcpy(id[index], fields[0]);
    strcpy(name[index], fields[1]);
    subcount[index] = subjectsToTeach;
    learncount[index] = subjectsToLearn;
    return 1;
}

int alrdyExist(char id[][15], int count, char searchId[])
{
    for (int i = 0; i < count; i++) {
        if (sameline(id[i], searchId))
            return 1;
    }
    return 0;
}

int loadStudents(char name[][NAME_LENGTH], char id[][15],
                 char subs[][MAX_SUB][SUB_LEN], int subcount[],
                 char learn[][MAX_SUB][SUB_LEN], int learncount[])
{
    FILE *file = fopen("students.txt", "r");// For demo purpouses, If file can not open or error
    if (file == NULL) {
        char first[] = "26K-0001|Taha Mayet|2|Calculus,C Programming|1|Functional English";
        char second[] = "26K-0002|Ahmed Kazmi|1|Functional English|2|calculus,C Programming";
       
        if(!readStudent(first, 0, name, id, subs, subcount, learn, learncount)|| !readStudent(second, 1, name, id, subs, subcount, learn, learncount))
            return -1;
        printf("Using sample students. Could not open students.txt.\n");
        return 2;
    }

    char line[L_LEN];
    int count = 0, lineNumber = 0;
    while(1) {
        int result = readLine(file, line);
        if(result == 0)
            break;

        lineNumber++;
        if  (result == 1 && line[0] == '\0')
            continue;

        if(result < 0 || count == MAX_STU|| !readStudent(line, count, name, id, subs, subcount, learn, learncount)) {
            printf("Invalid record or too many students at line %d.\n", lineNumber);
            count = -1;
            break;
        }
    if(alrdyExist(id, count, id[count])) {
            printf("Duplicate student ID on line %d.\n", lineNumber);
            count = -1;
            break;
        }
        count++;
    }
if(ferror(file)){
        puts("Could not read students.txt.");
        count = -1;
    }
    fclose(file);
    return count;
}
//will use fprintf
void printSubjects(FILE *file, char subjects[][SUB_LEN], int count)
{
    if  (count <= 0){
        return;
    }

    fprintf(file, "%s", subjects[0]);

    for(int i = 1; i < count; i++) {
        fprintf(file, ",%s", subjects[i]);
    }
}

int save(int count, char name[][NAME_LENGTH], char id[][15],char subs[][MAX_SUB][SUB_LEN], int subcount[],char learn[][MAX_SUB][SUB_LEN], int learncount[])
{
    char tempName[40] = "students.tmp";
    char saveName[40] = "students.txt";
    FILE *file = fopen(tempName, "w");

    if(file == NULL) {
        puts("Could not save students.txt.");
        return 0;
    }

for(int i = 0; i < count; i++) {
        fprintf(file, "%s|%s|%d|", id[i], name[i], subcount[i]);
        printSubjects(file, subs[i], subcount[i]);
        fprintf(file, "|%d|", learncount[i]);
        printSubjects(file, learn[i], learncount[i]);
        fputc('\n', file);
    }

    int failed = ferror(file);
    if(fclose(file) != 0)
        failed = 1;
    if(failed||rename(tempName, saveName) != 0) {
        remove(tempName);
        puts("Could not save students.txt.");
        return 0;
    }

    printf("Saved %d student record(s) to %s.\n", count, saveName);
}

void Liststudents(int count, char name[][NAME_LENGTH], char id[][15],char subs[][MAX_SUB][SUB_LEN], int subcount[],char learn[][MAX_SUB][SUB_LEN], int learncount[])
{
    printf("\nTotal students: %d\n", count);
    for (int i = 0; i < count; i++) {
        printf ("\n%s - %s\n", id[i], name[i]);
    printf("  Can teach: ");
        if(subcount[i] == 0)
            printf("None");
    printSubjects(stdout, subs[i], subcount[i]);

    printf("\n  Needs help: ");
        if(learncount[i] == 0)
            printf("None");
        printSubjects(stdout, learn[i], learncount[i]);
     printf("\n\n");
    }
}

int hasSubject(char subjects[][SUB_LEN], int count, char subject[]){
    int i = 0;

while(i < count) {
    if (sameline(subjects[i], subject) == 1) {
         return 1;
        }

        i++;
    }

return 0;
}

void showSubjectReport(int count, char subs[][MAX_SUB][SUB_LEN], int subcount[],char learn[][MAX_SUB][SUB_LEN], int learncount[])
{
char subject[L_LEN];
int canteach = 0;
int needsHelp = 0;
printf("Enter a subject: \n");

    if   (readLine(stdin, subject) != 1 || !validSubject(subject)) {
        puts("Enter a valid subject name.");
        return;
    }

    for(int i = 0; i < count; i++) {
        if  (hasSubject(subs[i], subcount[i], subject))
            canteach++;
        if(hasSubject(learn[i], learncount[i], subject))
            needsHelp++;
    }
printf("\nTotal students: %d\n", count);
printf("Can teach %s: %d\n", subject, canteach);
printf("Needs help with %s: %d\n", subject, needsHelp);
}

int main(void){
     
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
    int exit = 0;     

  count = loadStudents(name, id, subs, subcount, learn, learncount);

    if(count < 0) {
        exit = 1;     
        running = 0;
    } else {
        printf("Loaded %d student record.\n", count);
    }

    while  (running == 1) {
     printf("\n\n   STUDENT HELP   \n\n");
    printf("1. List all students\n");
    printf("2. Subject report\n");
     printf("3. Save students\n");
    printf("0. Exit\n");
    printf("Choose an option: ");

    result = readLine(stdin, choice);

        if(result == 0) {
            running = 0;   
        } else if(result < 0 || strlen(choice) != 1) {
            printf("Invalid choice. Enter 0, 1, 2, or 3.\n");
    }else if(choice[0] == '1') {
            Liststudents(count, name, id, subs, subcount, learn, learncount);
    }else if(choice[0] == '2') {
            showSubjectReport(count, subs, subcount, learn, learncount);
        } else if(choice[0] == '3') {
            save(count, name, id, subs, subcount,learn, learncount);
        }
        else if(choice[0] == '0') {
            running = 0;   
        }else{printf("Invalid choice. Enter 0, 1, 2, or 3.\n");
        }
    }

return exit;
}
