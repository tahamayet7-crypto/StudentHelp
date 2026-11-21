#include <stdio.h>
#define max_std 100
#define max_sub 5
#define max_name 50
#define max_sub_len 30
struct Student {
    int id;
    char name[max_name];
    char teach[max_sub][max_sub_len];
    int teachCount;
    char need[max_sub][max_sub_len];
    int needCount;
};
struct Student students[max_std];
int studentCount = 0;
void readSubjects(char subjects[][max_sub_len], int *count){
    printf("How many subjects? (0-5): ");
    scanf("%d", count);
    getchar();
    for (int i = 0; i < *count; i++) {
        printf("Enter subject %d: ", i + 1);
        fgets(subjects[i], max_sub_len, stdin);
    }
}
int findStudent(int id){
    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id)
            return i;
    }
    return -1;
}
void displayStudent(int index){
    printf("\nID: %d\n", students[index].id);
    printf("Name: %s", students[index].name);
    printf("Can teach:\n");
    for (int i = 0; i < students[index].teachCount; i++) {
        printf("%s", students[index].teach[i]);
    }
    printf("Needs help in:\n");
    for (int i = 0; i < students[index].needCount; i++) {
        printf("%s", students[index].need[i]);
    }
    printf("\n");
}
void registerStudent(){
    if (studentCount == max_std) {
        printf("Student list is full!\n");
        return;
    }
    int id;
    printf("\nEnter student ID: ");
    scanf("%d", &id);
    getchar();
    if (findStudent(id) != -1) {
        printf("ID already exists!\n");
        return;
    }
    students[studentCount].id = id;
    printf("Enter student name: ");
    fgets(students[studentCount].name, max_name, stdin);
    printf("\nSubjects the student can teach:\n");
    readSubjects(students[studentCount].teach, &students[studentCount].teachCount);
    printf("\nSubjects the student needs help in:\n");
    readSubjects(students[studentCount].need, &students[studentCount].needCount);
    studentCount++;
    printf("\nStudent registered successfully!\n");
}
void updateProfile()
{
    int id, index, choice;
    printf("\nEnter student ID to update: ");
    scanf("%d", &id);
    getchar();
    index = findStudent(id);
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
                fgets(students[index].name, max_name, stdin);
                break;
            case 2:
                readSubjects(students[index].teach, &students[index].teachCount);
                break;
            case 3:
                readSubjects(students[index].need, &students[index].needCount);
                break;
            case 4:
                printf("Finished updating.\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 4);
}
void removeProfile(){
    int id, index;
    printf("\nEnter student ID to remove: ");
    scanf("%d", &id);
    index = findStudent(id);
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
void displayAll(){
    if (studentCount == 0) {
        printf("No students registered.\n");
        return;
    }
    for (int i = 0; i < studentCount; i++) {
        printf("\nStudent %d\n", i + 1);
        displayStudent(i);
    }
}
int main(){
    int choice;
    do {
        printf("\n===== StudentHelp =====\n");
        printf("1. Register student\n");
        printf("2. Update profile\n");
        printf("3. Remove profile\n");
        printf("4. Display all students\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();
        switch (choice) {
            case 1:
                registerStudent();
                break;
            case 2:
                updateProfile();
                break;
            case 3:
                removeProfile();
                break;
            case 4:
                displayAll();
                break;
            case 0:
                printf("Byee!\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 0);
    return 0;
}
