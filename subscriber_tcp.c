// subscriber_tcp.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SZ 1024

static void die(const char *m){ perror(m); exit(EXIT_FAILURE); }

int main(int argc, char *argv[]){
    const char *server_ip = (argc >= 2) ? argv[1] : "127.0.0.1";

    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) die("socket");

    struct sockaddr_in addr; memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, &server_ip[0], &addr.sin_addr) <= 0) die("inet_pton");

    if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) die("connect");

    // Enviar rol
    char role = 'S';
    if (send(s, &role, 1, 0) != 1) die("send role");

    printf("[SUB] Suscrito a %s:%d. Esperando mensajes...\n", server_ip, PORT);

    char buf[BUF_SZ];
    while (1) {
        ssize_t n = recv(s, buf, sizeof(buf)-1, 0);
        if (n <= 0) { printf("[SUB] Conexión cerrada.\n"); break; }
        buf[n] = '\0';
        fputs(buf, stdout);
        fflush(stdout);
    }

    close(s);
    return 0;
}

