#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "fileutils.h"

FILE *createAndLoadCsv() {
    if (access("assets/feedback.csv", F_OK) != 0) {
        FILE *file = fopen("assets/feedback.csv", "w");
        fputs("Name,Teacher,Rating\n", file);
        fclose(file);
    }

    FILE *file = fopen("assets/feedback.csv", "a");
    return file;
}

void insertFeedback(FILE *file, char *name, char *teacher, long rating) {
    fprintf(file, "%s,%s,%ld\n", name, teacher, rating);
    fflush(file);
}

char **loadTeachers() {
    FILE *file = fopen("assets/teachers.txt", "r");

    char buf[128];
    char **teachers = malloc(sizeof(char *) * MAX_TEACHERS);

    int i = 0;
    while (fgets(buf, sizeof(buf), file) != NULL) {
        if (i >= MAX_TEACHERS) {
            printf("Warning: Maximum teachers (%d) reached, ignoring further values.\n",
                   MAX_TEACHERS);
            break;
        }

        int length = strlen(buf);
        buf[--length] = 0;
        char *teacher = malloc(sizeof(char) * length);

        strncpy(teacher, buf, length);
        teachers[i++] = teacher;
    }

    return teachers;
}
