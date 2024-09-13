#ifndef _SERVER_HANDLE_H
#define _SERVER_HANDLE_H

#define PORT 8000
#define MAX_CLIENT 10

struct Client {
    int client_fd;
    char *name;
    int teacherNumber;
};

typedef void (*S_CALLBACK)(struct Client *, char *, int);

int createServer();

struct Client *createClient(int client_fd);
void destroyClient(struct Client *client);
void messageClient(struct Client *client, char *message);
void disconnectClient(struct Client *client);

void startServer(int server_fd, S_CALLBACK callback);

#endif
