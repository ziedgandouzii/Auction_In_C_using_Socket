#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int main()
{

    int clientSocket, ret;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket < 0)
    {
        printf("[-]Erreur de connection.\n");
        exit(1);
    }
    printf("[+]Client Socket est crée.\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret < 0)
    {
        printf("[-]Erreur de connection.\n");
        exit(1);
    }
    printf("[+]Connecté au Serveur.\n");
    printf("+++L'enchère a Commencé.+++\n");
    while(1)
    {
        printf("Saisir un montant: \t");
        scanf("%s", &buffer[0]);
        send(clientSocket, buffer, strlen(buffer), 0);

        if(strcmp(buffer, "exit") == 0)
        {
            close(clientSocket);
            printf("[-]Déconnecté au server.\n");
            exit(1);
        }

    }

    return 0;
}
