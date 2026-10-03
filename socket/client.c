#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 4096

int main(void)
{
    // --------------------------------------------------
    // 1. Create socket
    // --------------------------------------------------

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (client_fd == -1) {

        perror("socket");
        return 1;
    }

    // --------------------------------------------------
    // 2. Configure server address
    // --------------------------------------------------

    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;

    server_addr.sin_port = htons(PORT);

    // Server is running on localhost
    if (inet_pton(AF_INET, "127.0.0.1",
                  &server_addr.sin_addr) <= 0) {

        perror("inet_pton");
        close(client_fd);
        return 1;
    }

    // --------------------------------------------------
    // 3. Connect to server
    // --------------------------------------------------

    printf("Connecting to server...\n");

    if (connect(client_fd,
                (struct sockaddr *)&server_addr,
                sizeof(server_addr)) == -1) {

        perror("connect");
        close(client_fd);
        return 1;
    }

    printf("Connected to server!\n");
    printf("Type messages. Type 'exit' to quit.\n\n");

    // --------------------------------------------------
    // 4. Communication loop
    // --------------------------------------------------

    char buffer[BUFFER_SIZE];

    while (1) {

        printf("You: ");

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }

        // Send message
        ssize_t bytes_sent = send(
            client_fd,
            buffer,
            strlen(buffer),
            0
        );

        if (bytes_sent == -1) {

            perror("send");
            break;
        }

        // Exit
        if (strncmp(buffer, "exit", 4) == 0) {
            break;
        }

        // --------------------------------------------------
        // Receive server response
        // --------------------------------------------------

        memset(buffer, 0, sizeof(buffer));

        ssize_t bytes_received = recv(
            client_fd,
            buffer,
            sizeof(buffer) - 1,
            0
        );

        if (bytes_received == -1) {

            perror("recv");
            break;
        }

        if (bytes_received == 0) {

            printf("Server disconnected.\n");
            break;
        }

        buffer[bytes_received] = '\0';

        printf("Server: %s", buffer);
    }

    // --------------------------------------------------
    // 5. Close socket
    // --------------------------------------------------

    close(client_fd);

    printf("Connection closed.\n");

    return 0;
}