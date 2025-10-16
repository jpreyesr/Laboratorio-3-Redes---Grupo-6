#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BROKER_PORT 5000
#define BUFFER_SIZE 1024

int main(){
    int sockfd;
    struct sockaddr_in broker_addr;
    char buffer[BUFFER_SIZE];

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("Error creando socket UDP");
        exit(EXIT_FAILURE);
    }

    memset(&broker_addr, 0, sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(BROKER_PORT);
    broker_addr.sin_addr.s_addr = inet_addr("192.168.64.3");

    printf("Publisher listo. Escribe mensajes sobre un partido (Ctrl+C para salir)\n");

    while(1){
        printf("> ");
        fflush(stdout);

        if(fgets(buffer, BUFFER_SIZE, stdin) == NULL)
            break;

        buffer[strcspn(buffer, "\n")] = '\0';
        printf("enviando mensaje al broker: %s\n", buffer);
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&broker_addr, sizeof(broker_addr));
    }

    close(sockfd);
    return 0;
}
