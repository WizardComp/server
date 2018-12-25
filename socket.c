//
// Created by animagik on 12/21/18.
//

#include <memory.h>

#include <assert.h>
#include <unistd.h>
#include <time.h>
#include <netdb.h>
#include <fcntl.h>

#include "socket.h"


int
socket_listen_server(const char *port) {
    struct addrinfo *result = NULL, hints;
    int ret;
    int option = 1;
    int listen_socket;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    ret = getaddrinfo(NULL, port, &hints, &result);
    if (ret != 0) {
        fprintf(stderr, "[server] getaddrinfo failed: %d\n", ret);
        return -1;
    }
    listen_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listen_socket == -1) {
        fprintf(stderr, "[server] error at socket(): %i\n", errno);
        freeaddrinfo(result);
        return -1;
    }
    setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, (const char *) &option, sizeof(option));
    setsockopt(listen_socket, SOL_SOCKET, SO_DEBUG, (const char *) &option, sizeof(option));
    ret = bind(listen_socket, result->ai_addr, (int) result->ai_addrlen);
    if (ret == -1) {
        fprintf(stderr, "[server] bind failed with error: %d\n", errno);
        freeaddrinfo(result);
        close(listen_socket);
        return -1;
    }
    if (listen(listen_socket, SOMAXCONN) == -1) {
        fprintf(stderr, "[server] listen failed with error: %i\n", errno);
        close(listen_socket);
        return -1;
    }
    freeaddrinfo(result);
    return listen_socket;
}

int
binary_to_hex(const uint8_t *binary, size_t binary_size, char *hex_text, size_t hex_text_size) {
    int i;

    if (binary == 0 || hex_text == 0 || binary_size == 0 || hex_text_size == 0)
        return 0;
    if (binary_size * 2 > hex_text_size)
        return 0;
    memset(hex_text, 0, hex_text_size);
    for (i = 0; i < binary_size; ++i) {
        int k = i * 2;
        hex_text[k] = hex[(binary[i] >> 4) & 0xF];
        hex_text[k + 1] = hex[(binary[i]) & 0xF];
    }
    return 1;
}

void output_hex(char *binary, size_t binary_size) {
    size_t hex_size = (binary_size * 2) + 1;
    char *hex = malloc(hex_size);
    assert(binary_to_hex(binary, binary_size, hex, hex_size) == 1);
    fprintf(stdout, "%s\n", hex);
    free(hex);
}

void client_read(void *object, void *user_data) {
    int s = (int) user_data;
    assert(object != 0);

    recv(s, (void *) registers, sizeof(registers), 0);
    output_hex(registers, sizeof(registers));


}

