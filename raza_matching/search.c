#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "studenthelp.h"


static int isSame(const char a[], const char b[])
{
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (tolower((unsigned char)a[i]) != tolower((unsigned char)b[i]))
            return 0;
        i++;
    }
    return a[i] == b[i];
}

static int checkTeach(const Student *s, const char subject[])
{
    int i;
    for (i = 0; i < s->teachCount; i++) {
        if (isSame(s->teach[i], subject))
            return 1;
    }
    return 0;
}

static int countHelp(const Student *teacher, const Student *need)
{
    int i, covered = 0;
    for (i = 0; i < need->needCount; i++) {
        if (checkTeach(teacher, need->need[i]))
            covered++;
    }
    return covered;
}

int findStudent(Student list[], int count, const char id[])
{
    int i;
    for (i = 0; i < count; i++) {
        if (isSame(list[i].id, id))
            return i;
    }
    return -1;
}

void searchTeacher(Student list[], int count, char subject[])
{
    int i, found = 0;

    printf("\n--- Students who can teach \"%s\" ---\n", subject);
    for (i = 0; i < count; i++) {
        if (checkTeach(&list[i], subject)) {
            printf("  %s (%s)\n", list[i].name, list[i].id);
            found++;
        }
    }
    if (found == 0)
        printf("  No student teaches this subject yet.\n");
    else
        printf("Total found: %d\n", found);
}

void showMatch(Student list[], int count, int index)
{
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

    printf("\n--- Matches for %s (%s) ---\n", me->name, me->id);

    printf("Can teach everything you need:\n");
    for (i = 0; i < count; i++) {
        if (i == index)
            continue;
        if (countHelp(&list[i], me) == me->needCount) {
            printf("  %s (%s)\n", list[i].name, list[i].id);
            fullFound++;
        }
    }
    if (fullFound == 0)
        printf("  None.\n");

    printf("Can teach part of what you need:\n");
    for (i = 0; i < count; i++) {
        if (i == index)
            continue;
        covered = countHelp(&list[i], me);
        if (covered > 0 && covered < me->needCount) {
            printf("  %s (%s) - covers %d of %d: ",
                   list[i].name, list[i].id, covered, me->needCount);
            for (j = 0; j < me->needCount; j++) {
                if (checkTeach(&list[i], me->need[j]))
                    printf("%s ", me->need[j]);
            }
            printf("\n");
            partialFound++;
        }
    }
    if (partialFound == 0)
        printf("  None.\n");
}

void showPartners(Student list[], int count)
{
    int a, b, i, found = 0;

    printf("\n--- Mutual Study Partners ---\n");
    for (a = 0; a < count; a++) {
        for (b = a + 1; b < count; b++) {

            int aToB = countHelp(&list[a], &list[b]);
            int bToA = countHelp(&list[b], &list[a]);

            if (aToB > 0 && bToA > 0) {
                found++;
                printf("%d) %s (%s)  <->  %s (%s)\n", found,
                       list[a].name, list[a].id, list[b].name, list[b].id);

                printf("     %s teaches %s: ", list[a].name, list[b].name);
                for (i = 0; i < list[b].needCount; i++) {
                    if (checkTeach(&list[a], list[b].need[i]))
                        printf("%s ", list[b].need[i]);
                }
                printf("\n");
                printf("     %s teaches %s: ", list[b].name, list[a].name);
                for (i = 0; i < list[a].needCount; i++) {
                    if (checkTeach(&list[b], list[a].need[i]))
                        printf("%s ", list[a].need[i]);
                }
                printf("\n");
            }
        }
    }
    if (found == 0)
        printf("  No mutual study partners found.\n");
}
