// publisher_tcp.c
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
    if (inet_pton(AF_INET, server_ip, &addr.sin_addr) <= 0) die("inet_pton");

    if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) die("connect");

    // Enviar rol
    char role = 'P';
    if (send(s, &role, 1, 0) != 1) die("send role");

    printf("[PUB] Conectado a %s:%d. Escribe líneas y Enter para publicar. Ctrl+D para salir.\n",
           server_ip, PORT);

    char line[BUF_SZ];
    while (fgets(line, sizeof(line), stdin)) {
        size_t len = strlen(line);
        if (send(s, line, len, 0) < 0) { perror("send"); break; }
    }

    close(s);
    return 0;
}

