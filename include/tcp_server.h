/**
 * Project: micro_message
 * Author: Carlo Di Giuseppe
 * Date: 20/06/25
 * License: MIT
 */
 
#ifndef TCP_SERVER_H
#define TCP_SERVER_H

typedef void (*on_message_fn)(int socket, const char* message);

int tpc_server_start(unsigned int port, char* name, on_message_fn on_message);

void on_admin_message(int socket, const char *message);
void on_microservice_message(int socket, const char *message);

#endif //TCP_SERVER_H
