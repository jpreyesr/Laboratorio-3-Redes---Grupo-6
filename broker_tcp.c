// broker_tcp.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>

#define PORT 8080
#define MAX_CLIENTS 64
#define BUF_SZ 1024

typedef enum { ROLE_UNKNOWN=0, ROLE_PUB=1, ROLE_SUB=2 } role_t;

static void die(const char *msg){ perror(msg); exit(EXIT_FAILURE); }

int main() {
    int server_fd, opt = 1;
    struct sockaddr_in addr;
    int client_fd[MAX_CLIENTS];
    role_t client_role[MAX_CLIENTS];
    int i;

    for (i = 0; i < MAX_CLIENTS; ++i){ client_fd[i] = -1; client_role[i] = ROLE_UNKNOWN; }

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) die("socket");

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        die("setsockopt");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) die("bind");
    if (listen(server_fd, 10) < 0) die("listen");

    printf("[BROKER] Escuchando en %d...\n", PORT);

    while (1) {
        fd_set rfds; FD_ZERO(&rfds);
        FD_SET(server_fd, &rfds);
        int fdmax = server_fd;

        for (i = 0; i < MAX_CLIENTS; ++i) {
            if (client_fd[i] != -1) {
                FD_SET(client_fd[i], &rfds);
                if (client_fd[i] > fdmax) fdmax = client_fd[i];
            }
        }

        int ready = select(fdmax + 1, &rfds, NULL, NULL, NULL);
        if (ready < 0) die("select");

        // Nueva conexión
        if (FD_ISSET(server_fd, &rfds)) {
            int cfd = accept(server_fd, NULL, NULL);
            if (cfd < 0) { perror("accept"); continue; }

            // Busca un hueco
            int idx = -1;
            for (i = 0; i < MAX_CLIENTS; ++i) if (client_fd[i] == -1) { idx = i; break; }
            if (idx == -1) {
                fprintf(stderr, "[BROKER] Sin espacio, cerrando cliente\n");
                close(cfd);
            } else {
                client_fd[idx] = cfd;
                client_role[idx] = ROLE_UNKNOWN;
                // leer 1 byte de rol
                char r;
                ssize_t n = recv(cfd, &r, 1, MSG_WAITALL);
                if (n <= 0) { close(cfd); client_fd[idx] = -1; continue; }
                if (r == 'P') client_role[idx] = ROLE_PUB;
                else if (r == 'S') client_role[idx] = ROLE_SUB;
                else { client_role[idx] = ROLE_UNKNOWN; }

                printf("[BROKER] Cliente %d conectado como %s\n",
                       cfd,
                       client_role[idx]==ROLE_PUB?"PUBLISHER":
                       client_role[idx]==ROLE_SUB?"SUBSCRIBER":"DESCONOCIDO");
            }
        }

        // Datos de clientes
        for (i = 0; i < MAX_CLIENTS; ++i) {
            int cfd = client_fd[i];
            if (cfd == -1) continue;
            if (!FD_ISSET(cfd, &rfds)) continue;

            char buf[BUF_SZ];
            ssize_t n = recv(cfd, buf, sizeof(buf)-1, 0);
            if (n <= 0) {
                // desconexión
                printf("[BROKER] Cliente %d desconectado\n", cfd);
                close(cfd);
                client_fd[i] = -1;
                client_role[i] = ROLE_UNKNOWN;
                continue;
            }
            buf[n] = '\0';

            if (client_role[i] == ROLE_PUB) {
                // reenviar a todos los SUB
                for (int j = 0; j < MAX_CLIENTS; ++j) {
                    if (client_fd[j] != -1 && client_role[j] == ROLE_SUB) {
                        ssize_t s = send(client_fd[j], buf, n, 0);
                        if (s < 0) { perror("send"); }
                    }
                }
            } else {
                // Ignorar datos de SUB (no deberían mandar nada)
            }
        }
    }

    close(server_fd);
    return 0;
}

