#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
int main()
{
    int sockfd, ret,i=0,client,montant_max=0,montant;
    struct sockaddr_in serverAddr; //enregistrement contient la famille de serveur et la num de port
    int newSocket;
    struct sockaddr_in newAddr;

    socklen_t addr_size; // longeur de valeur dans la socket

    char buffer[1024],buffer1[20]; // taille de pipe qui assurer la cominucation entre serveur et clien
    pid_t childpid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        printf("[-]Erreur de connection.\n");
        exit(1);
    }
    printf("[+]Serveur de Socket est Crée avec succée.\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret < 0)
    {
        printf("[-]Erreur de Liaison.\n");
        exit(1);
    }
    printf("[+]Lier à 127.0.0.1: %d\n",8080);

    if(listen(sockfd, 10) == 0)
    {
        printf("[+]Atteindre....\n");
    }
    else
    {
        printf("[-]Erreur de Liaison.\n");
    }

    printf("+++Enchère+++\n");
    while(1)
    {
        newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
        if(newSocket < 0)
        {
            exit(1);
        }
        printf("Connection accepté de %s :%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
        printf("Nombre Totale de Client maintenant est =%d\n",++i);
        if((childpid = fork()) == 0)
        {
            close(sockfd);

            while(1)
            {
                bzero(buffer, sizeof(buffer));
                recv(newSocket, buffer, 1024, 0);
                if(strcmp(buffer, ":exit") == 0)
                {
                    printf("Déconnecté de %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
                    break;
                }
                else
                {
                    printf("Le client %i a offert un montant de :%s DT \n",i,buffer);
                    montant=atoi(buffer);//convertir chaine to entier
                    if(montant>montant_max)
                    {
                        montant_max=montant;
                        client=i;//numero de client qui a proposé le montant max
                    }
                    bzero(buffer, sizeof(buffer));
                }
                printf("Ecrire 'stop' pour arreter l'enchaire,sinon tapez 'c'\n");
                scanf("%s",&buffer1);
                if(strcmp(buffer1,"stop")==0)
                {
                    printf("Le gagnant de l'enchère est le client %d pour un montant de %d DT",client,montant_max);
                    exit(1);
                }
            }
        }

    }

    close(newSocket);


    return 0;
}
