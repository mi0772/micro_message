/**
 * Project: micro_message
 * Author: Carlo Di Giuseppe
 * Date: 20/06/25
 * License: MIT
 */
 
#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/_endian.h>
#include <unistd.h>

#define INVALID_COMMAND -1

typedef enum {
    SUBSCRIBE = 0,
    UNSUBSCRIBE,
    PUBLISH,
} micro_command_e;

typedef void (*on_message_fn)(int socket, struct sockaddr_in *client, socklen_t len_client, const micro_command_e command);

int tcp_server_start(unsigned int port, char* name, on_message_fn on_message);

void on_admin_message(int socket, struct sockaddr_in *client, socklen_t len_client, const micro_command_e command);
void on_microservice_message(int socket, struct sockaddr_in *client, socklen_t len_client, const micro_command_e command);
micro_command_e parse_command(const char *message);

#endif //UDP_SERVER_H
