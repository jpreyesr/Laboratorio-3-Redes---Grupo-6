#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#define BROKER_PORT 5000
#define MAX_SUBSCRIBERS 10
#define BUFFER_SIZE 1024

typedef struct {
    struct sockaddr_in addr;
    char topic[50];
} Subscriber;

int main(){
    int sockfd;
    struct sockaddr_in broker_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len;
    Subscriber subscribers[MAX_SUBSCRIBERS];
    int num_subscribers = 0;

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("Error creando socket UDP");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
        perror("socket fallo");
    }

    memset(&broker_addr, 0, sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_addr.s_addr = INADDR_ANY;
    broker_addr.sin_port = htons(BROKER_PORT);

    if(bind(sockfd, (const struct sockaddr *)&broker_addr, sizeof(broker_addr)) < 0){
        perror("Error en bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Broker UDP escuchando en puerto %d...\n", BROKER_PORT);
    system("sudo netstat -ulnp | grep 5000");

    while(1){
        addr_len = sizeof(client_addr);
        ssize_t n = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr *)&client_addr, &addr_len);

        if(n <= 0){
            perror("Error o datagrama vacio al recibir mensaje");
            printf("Codigo errno: %d\n", errno);
            printf("direccion ip origen (posible): %s\n", inet_ntoa(client_addr.sin_addr));
            sleep(1);
            continue;
        }

        buffer[n] = '\0';

        if(strncmp(buffer, "SUBSCRIBE", 9) == 0){
            char topic[50];
            sscanf(buffer, "SUBSCRIBE %s", topic);
            strcpy(subscribers[num_subscribers].topic, topic);
            subscribers[num_subscribers].addr = client_addr;
            num_subscribers++;

            printf("Nuevo suscriptor (%s) registrado para %s\n",
                   inet_ntoa(client_addr.sin_addr), topic);
        }
        else{
            printf("Mensaje recibido: %s\n", buffer);

            for(int i = 0; i < num_subscribers; i++){
                if(strstr(buffer, subscribers[i].topic)){
                    ssize_t sent = sendto(sockfd, buffer, strlen(buffer), 0,
                                          (struct sockaddr *)&subscribers[i].addr,
                                          sizeof(subscribers[i].addr));
                    if(sent < 0)
                        perror("error enviando mensaje a suscriptor");
                }
            }
        }
    }

    close(sockfd);
    return 0;
}
