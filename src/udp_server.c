/**
 * Project: micro_message
 * Author: Carlo Di Giuseppe
 * Date: 20/06/25
 * License: MIT
 */

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/_endian.h>
#include <unistd.h>
#include "udp_server.h"
#include "string_man.h"

static void sanitize_buffer(char *buffer);

int udp_server_start(unsigned int port, char *name, on_message_fn on_message) {
    char buffer[1024];

    int server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in address, client;
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

    printf("%s server is running and listening on port %d (UDP)...\n", name, port);

    while (1) {
        socklen_t len = sizeof(client);
        ssize_t n = recvfrom(server_fd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *) &client, &len);
        if (n < 0) {
            perror("recvfrom failed");
            continue;
        }
        buffer[n] = '\0'; // null-terminate
        sanitize_buffer(buffer);

        micro_command_e command = parse_command(buffer);
        if (command != INVALID_COMMAND) {
            on_message(server_fd, &client, len, command);
        } else {
            printf("Invalid command received: %s\n", buffer);
        }
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
    sendto(socket, r, strlen(r), 0, (struct sockaddr *) client, len_client);
    // Here you can handle the admin message as needed
}

void on_microservice_message(int socket, struct sockaddr_in *client, socklen_t len_client, const micro_command_e command) {
    printf("Microservice message received: %d\n", command);
    // Here you can handle the microservice message as needed
    char *r = "ACK";
    sendto(socket, r, strlen(r), 0, (struct sockaddr *) client, len_client);
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