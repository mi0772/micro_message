/**
 * Project: micro_message
 * Author: Carlo Di Giuseppe
 * Date: 20/06/25
 * License: MIT
 */

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "string_man.h"
#include "tcp_server.h"

static void sanitize_buffer(char *buffer);

int tcp_server_start(unsigned int port, char *name, on_message_fn on_message) {
    char buffer[1024];

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    int rb = bind(server_fd, (struct sockaddr *) &address, sizeof(address));
    if (rb < 0) {
        perror("bind failed");
        return -1;
    }

    if (listen(server_fd, SOMAXCONN) < 0) {
        perror("listen failed");
        return -1;
    }

    printf("%s server is running and listening on port %d (TCP)...\n", name, port);

    while (1) {
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        int client_fd = accept(server_fd, (struct sockaddr *)&client, &len);
        if (client_fd < 0) {
            perror("accept failed");
            continue;
        }

        ssize_t n = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (n < 0) {
            perror("recv failed");
            close(client_fd);
            continue;
        }
        buffer[n] = '\0'; // null-terminate
        sanitize_buffer(buffer);

        micro_command_e command = parse_command(buffer);
        if (command != INVALID_COMMAND) {
            on_message(client_fd, &client, len, command);
        } else {
            printf("Invalid command received: %s\n", buffer);
        }

        close(client_fd);
    }
}

micro_command_e parse_command(const char *message) {
    if (message == NULL) return -1; // Invalid input

    to_lower(message);

    if (strcmp(message, "subscribe") == 0) return SUBSCRIBE;
    if (strcmp(message, "unsubscribe") == 0) return UNSUBSCRIBE;
    if (strcmp(message, "publish") == 0) return PUBLISH;

    return INVALID_COMMAND; // Invalid command
}

void on_admin_message(int socket, struct sockaddr_in *client, socklen_t len_client, const micro_command_e command) {
    printf("Admin message received: %d\n", command);
    char *r = "ACK";
    send(socket, r, strlen(r), 0);
    // Here you can handle the admin message as needed
}

void on_microservice_message(int socket, struct sockaddr_in *client, socklen_t len_client, const micro_command_e command) {
    printf("Microservice message received: %d\n", command);
    // Here you can handle the microservice message as needed
    char *r = "ACK";
    send(socket, r, strlen(r), 0);
}

static void sanitize_buffer(char *buffer) {
    char *src = buffer;
    char *dst = buffer;

    while (*src) {
        if (*src != '\n' && *src != '\r') *dst++ = *src;
        src++;
    }
    *dst = '\0'; // Null-terminate the sanitized string
}