#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BROKER_PORT 5000
#define BUFFER_SIZE 1024

int main(){
    int sockfd;
    struct sockaddr_in broker_addr, local_addr, sender_addr;
    char buffer[BUFFER_SIZE];
    char topic[50];
    socklen_t addr_len = sizeof(broker_addr);

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("Error creando el socket UDP");
        exit(EXIT_FAILURE);
    }

    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = 0;

    if(bind(sockfd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0){
        perror("Error en bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    memset(&broker_addr, 0, sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(BROKER_PORT);
    broker_addr.sin_addr.s_addr = inet_addr("192.168.64.3");

    printf("Ingrese el tema a suscribirse (por ejemplo, equipoA): ");
    fgets(topic, sizeof(topic), stdin);
    topic[strcspn(topic, "\n")] = '\0';

    snprintf(buffer, sizeof(buffer), "SUBSCRIBE %s", topic);

    printf("enviando solicitud de suscripcion al broker (%s:%d): %s\n",
           inet_ntoa(broker_addr.sin_addr), ntohs(broker_addr.sin_port), buffer);

    ssize_t sent = sendto(sockfd, buffer, strlen(buffer), 0,
                          (struct sockaddr *)&broker_addr, sizeof(broker_addr));

    printf("suscrito al tema '%s'. Esperando mensajes...\n", topic);

    while(1){
        ssize_t n = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                             (struct sockaddr *)&sender_addr, &addr_len);

        if(n < 0){
            perror("error recibiendo mensaje");
            continue;
        }

        buffer[n] = '\0';
        printf("%s\n", buffer);
    }

    close(sockfd);
    return 0;
}
