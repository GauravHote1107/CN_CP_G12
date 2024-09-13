#ifndef _CSV_UTILS_H
#define _CSV_UTILS_H

#include <stdio.h>

#define MAX_TEACHERS 10

FILE *createAndLoadCsv();
void insertFeedback(FILE *file, char *name, char *teacher, long rating);

char **loadTeachers();

#endif