/**
 * Project: micro_message
 * Author: ${$USER_NAME}
 * Date: 20/06/25
 * License: MIT
 */

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/_endian.h>
#include <unistd.h>


#include "tcp_server.h"

int tpc_server_start(unsigned int port, char* name, on_message_fn on_message) {

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(port);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));

    listen(server_fd, 3);

    printf("%s server is running and listening on port %d ...\n", name, port);

    while (1) {
        int addrlen = sizeof(address);
        int new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

        if (new_socket < 0) {
            perror("Failed to accept connection");
            continue;
        }

        printf("Connection accepted.\n");

        char buffer[1024] = {0};
        ssize_t bytes_read = read(new_socket, buffer, sizeof(buffer) - 1);

        on_message(new_socket, buffer);

        //char *response = "Hello from server!";
        //write(new_socket, response, strlen(response));


        close(new_socket);   // Close the socket for this client
        printf("Connection closed.\n");
    }
}

void on_admin_message(int socket, const char *message) {
    printf("Admin message received: %s\n", message);
    // Here you can handle the admin message as needed
}

void on_microservice_message(int socket, const char *message) {
    printf("Microservice message received: %s\n", message);
    // Here you can handle the microservice message as needed
}