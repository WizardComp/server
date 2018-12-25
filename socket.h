

#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <memory.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include <netdb.h>
#include <fcntl.h>

#define MYPORT "3684"
static volatile char registers[100];

static const char hex[] = "0123456789ABCDEF";

int socket_listen_server(const char *port);

void client_read(void *object, void *user_data);

int binary_to_hex(const uint8_t *binary, size_t binary_size, char *hex_text, size_t hex_text_size);

void output_hex(char *binary, size_t binary_size);

#endif //SERVER_SOCKET_H
