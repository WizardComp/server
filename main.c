23

#include <stdbool.h>
#include <sys/time.h>
#include <pthread.h>
#include "monitor.h"
#include "socket.h"


struct Server {
    struct Monitor monitor;
    int client_socket;

};


void *simple_thread_procedure(void *serv) {
    struct Server *s = (struct Server *) serv;
    monitor_enter(&s->monitor, client_read, (void *) s->client_socket);

}
int main(int argc, char **argv) {
    struct sockaddr_in sock_addr_in;
    int sock_addr_len = sizeof(sock_addr_in);
    int listen_socket = socket_listen_server(MYPORT);


    struct Server serv;
    int counter = 0;
    monitor_init(&serv.monitor, &counter);
    while (true) {
        serv.client_socket = accept(listen_socket,
                                    (struct sockaddr *) &sock_addr_in,
                                    (socklen_t *) &sock_addr_len);
        if (serv.client_socket == -1) break;

            pthread_t thread;
               if(pthread_create(&thread, 0,simple_thread_procedure,(void *)&serv)==0){
                   pthread_join(thread,0);
               }else{
                   fprintf(stdout, "", errno);}


    }
    monitor_destroy(&serv.monitor);


    return EXIT_SUCCESS;
}
