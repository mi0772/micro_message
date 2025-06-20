#include <pthread.h>
#include <stdio.h>
#include "udp_server.h"
#include "version.h"

static void startup_banner(void);

void* admin_server_start_f(void* arg) {
    int port = *(int *)arg;
    udp_server_start(port, "admin", on_admin_message);
    return NULL;
}

void *message_server_start_f(void *arg) {
    int port = *(int *)arg;
    udp_server_start(port, "message", on_microservice_message);
    return NULL;
}

int main(void) {
    int message_server_port = 12018;
    int admin_server_port = 19976;

    startup_banner();

    pthread_t admin_server_thread, message_server_thread;
    pthread_create(&admin_server_thread, NULL, admin_server_start_f, &admin_server_port);
    pthread_create(&message_server_thread, NULL, message_server_start_f, &message_server_port);

    pthread_join(admin_server_thread, NULL);
    pthread_join(message_server_thread, NULL);

    return 0;
}

static void startup_banner(void) {
    printf("%s, version: %s, Build: %s, released on: %s\n", APP_NAME, APP_VERSION, APP_BUILD, APP_RELEASE_DATE);
}
