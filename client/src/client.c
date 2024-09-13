#include <arpa/inet.h> //sockaddr_in
#include <stdio.h> //printf
#include <stdlib.h> //connect
#include <string.h> //strlen
#include <sys/socket.h> //socket
#include <unistd.h> //read write

#define PORT 8000

void messageRecursive(int client_fd) {
    while (1) {
        char buf[128] = {0};
        read(client_fd, buf, sizeof(buf));
        if ((strncmp(buf, "exit", 4)) == 0) {
            printf("Info: Disconnected from server, connection closed.\n");
            break;
        }

        printf("%s", buf);
        int n = 0;
        while ((buf[n++] = getchar()) != '\n')
            ;
        buf[n] = 0;
        write(client_fd, buf, strlen(buf));
    }
}

int main() {
    int client_fd;
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error: Could not create the socket file descriptor.\n");
        exit(1);
    }


    struct sockaddr_in address;


    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(PORT);

    if (connect(client_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        printf("Error: Could not connect to the server.\n");
        exit(0);
    }

    messageRecursive(client_fd);
    close(client_fd);
}
