#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileutils.h"
#include "serverutils.h"

FILE *file;

char **teachers;
int teachersLength;

void sendTeacher(struct Client *client) {
    char buf[128];
    snprintf(buf, sizeof(buf), "Enter feedback for %s (1 - 10): ", teachers[client->teacherNumber]);

    messageClient(client, buf);
}




void callback(struct Client *client, char *message, int length) {
    if (client->teacherNumber == -1) {
        int nameLength = strlen(message);

        client->name = malloc(sizeof(char) * nameLength);
        strncpy(client->name, message, nameLength);

        client->teacherNumber++;
        sendTeacher(client);
        return;
    }



    long rating;
    char *end;

    rating = strtol(message, &end, 10);
    if (end == message || rating > 10 || rating < 0) {
        messageClient(client, "Error: Please enter a valid number (1 - 10) as a feedback.\n");
        sendTeacher(client);
        return;
    }

    insertFeedback(file, client->name, teachers[client->teacherNumber], rating);
    client->teacherNumber++;

    if (client->teacherNumber >= teachersLength) {
        messageClient(client, "Thanks for your feedback! We have noted it down successfully.\n");
        messageClient(client, "exit");
        disconnectClient(client);
        return;
    }

    sendTeacher(client);
}

int main(int argc, const char *argv[]) {
    file = createAndLoadCsv();
    teachers = loadTeachers();
    for (int i = 0; i < MAX_TEACHERS; i++) {
        if (teachers[i] == NULL) {
            teachersLength = i;
            break;
        }
    }

    int server_fd = createServer();
    startServer(server_fd, (S_CALLBACK)callback);
}
