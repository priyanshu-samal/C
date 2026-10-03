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
    // 1. Create server socket
    // --------------------------------------------------

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1) {
        perror("socket");
        return 1;
    }

    // Allow reuse of the port immediately after restarting
    int opt = 1;

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
                   &opt, sizeof(opt)) == -1) {
        perror("setsockopt");
        close(server_fd);
        return 1;
    }

    // --------------------------------------------------
    // 2. Configure server address
    // --------------------------------------------------

    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;

    // Accept connections on all local interfaces
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Convert port to network byte order
    server_addr.sin_port = htons(PORT);

    // --------------------------------------------------
    // 3. Bind socket to IP + port
    // --------------------------------------------------

    if (bind(server_fd,
             (struct sockaddr *)&server_addr,
             sizeof(server_addr)) == -1) {

        perror("bind");
        close(server_fd);
        return 1;
    }

    // --------------------------------------------------
    // 4. Start listening
    // --------------------------------------------------

    if (listen(server_fd, 10) == -1) {

        perror("listen");
        close(server_fd);
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    // --------------------------------------------------
    // 5. Accept client
    // --------------------------------------------------

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int client_fd = accept(
        server_fd,
        (struct sockaddr *)&client_addr,
        &client_len
    );

    if (client_fd == -1) {

        perror("accept");
        close(server_fd);
        return 1;
    }

    printf(
        "Client connected: %s:%d\n",
        inet_ntoa(client_addr.sin_addr),
        ntohs(client_addr.sin_port)
    );

    // --------------------------------------------------
    // 6. Communication loop
    // --------------------------------------------------

    char buffer[BUFFER_SIZE];

    while (1) {

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

        // Client disconnected
        if (bytes_received == 0) {

            printf("Client disconnected.\n");
            break;
        }

        buffer[bytes_received] = '\0';

        printf("Client: %s", buffer);

        // Exit command
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Closing connection...\n");
            break;
        }

        // --------------------------------------------------
        // Send response
        // --------------------------------------------------

        const char *response =
            "Message received by server!\n";

        ssize_t bytes_sent = send(
            client_fd,
            response,
            strlen(response),
            0
        );

        if (bytes_sent == -1) {

            perror("send");
            break;
        }
    }

    // --------------------------------------------------
    // 7. Close sockets
    // --------------------------------------------------

    close(client_fd);
    close(server_fd);

    return 0;
}