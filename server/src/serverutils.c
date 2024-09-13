#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "serverutils.h"

struct Client *client_socket[MAX_CLIENT] = {NULL};

int createServer() {
    int server_fd;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error: Could not create the socket file descriptor.\n");
        exit(1);
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        printf("Error: Could not mark the socket as reuseable.\n");
        exit(1);
    }

    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        printf("Error: Could not bind the socket to port %d, make sure the port is not in use.\n",
               PORT);
        exit(1);
    }

    return server_fd;
}



// CreateClient functiion creates Client
struct Client *createClient(int client_fd) {
    struct Client *client = malloc(sizeof(struct Client));

    client->client_fd = client_fd;
    client->teacherNumber = -1;

    return client;
}

void destroyClient(struct Client *client) {
    free(client->name);
    free(client);
}

void messageClient(struct Client *client, char *message) {
    send(client->client_fd, message, strlen(message), 0);
}



//to disconnect Client
void disconnectClient(struct Client *client) {
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    getpeername(client->client_fd, (struct sockaddr *)&address, &addrlen);
    printf("Info: Client disconnected with:\nIP: %s\nPort: %d\n", inet_ntoa(address.sin_addr),
           ntohs(address.sin_port));

    close(client->client_fd);

    for (int i = 0; i < MAX_CLIENT; i++) {
        if (client_socket[i] == client) {
            client_socket[i] = NULL;
            break;
        }
    }

    destroyClient(client);
}





void startServer(int server_fd, S_CALLBACK callback) {
    printf("Waiting for connections...\n");
    listen(server_fd, 3);

    fd_set read_fds;
    int fd, max_fd;

    int new_socket, activity;

    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(server_fd, &read_fds);

        max_fd = server_fd;
        for (int i = 0; i < MAX_CLIENT; i++) {
            fd = client_socket[i] == NULL ? 0 : client_socket[i]->client_fd;

            if (fd > 0)
                FD_SET(fd, &read_fds);

            if (fd > max_fd)
                max_fd = fd;
        }

        activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (activity < 0) {
            printf("Error: Could not select client socket for activity.\n");
            exit(1);
        }


//Code to add new Connection
        if (FD_ISSET(server_fd, &read_fds)) {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
                printf("Error: Could not accept new client.\n");
                exit(1);
            }

            printf("Info: New client accepted with:\nSocket FD: %d\nIP: %s\nPort: %d\n", new_socket,
                   inet_ntoa(address.sin_addr), ntohs(address.sin_port));

            char *message = "--- [Teacher Feedback System] ---\nEnter your name: ";
            send(new_socket, message, strlen(message), 0);

            for (int i = 0; i < MAX_CLIENT; i++)
                if (client_socket[i] == NULL) {
                    struct Client *client = createClient(new_socket);
                    client_socket[i] = client;
                    break;
                }
        }
//



//For message request
//It check who is sendig feedback
//It verifies whether client fd == read fd

        for (int i = 0; i < MAX_CLIENT; i++) {
            if (client_socket[i] == NULL)
                continue;

            fd = client_socket[i]->client_fd;
            if (FD_ISSET(fd, &read_fds)) {
                char buf[1024];
                int length;

                if ((length = read(fd, buf, sizeof(buf))) == 0) {
                    disconnectClient(client_socket[i]);
                } else {
                    buf[length - 1] = 0; //to remove pressing enter key as a feedback
                    (*callback)(client_socket[i], buf, length - 1);
                }
            }
        }
    }
}
