
// Client code in C to sort the array 
#include <arpa/inet.h> 
#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include <pthread.h>
#include "distributed_server.h"
#include "utils.h"
  #include <stdlib.h>

// Driver code 
int serverSocket, clientSock, centralSocket, clienteLength; 
int sock; 
struct sockaddr_in distributed_server, centralServer, client; 
int sensors[8] = {0,0,0,0,0,0,0,0};
int devices[QNT_DEVICES] = { 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 36, 5, 20, 7 };
int i, temp; 
pthread_t trecive, tsend;

void *read_data_distributed(void *params);
void *send_data_distributed(void *params);
void handleTCPClient(int socketCliente);

int main(int argc, char* argv[]) 
{ 
    initServerSocket(&sock, &distributed_server, IP, 10101, &clienteLength);
     
    puts("THREAD"); 
    pthread_create(&trecive, NULL, read_data_distributed, NULL);
    while (1)
    {
        pthread_create(&tsend, NULL, send_data_distributed, NULL);
        pthread_join (tsend, NULL);
        sleep(1);
    }
    
    pthread_join (trecive, NULL);
    close(centralSocket);
    printf("ACABOU O KO");

    return 0; 
} 


void *send_data_distributed(void *params){
    int flag = initSocketClient(&centralSocket, &centralServer, IP, 10001); 
    int values[3];
    values[0] = rand() % 16; 
    values[1] = rand() % 50; 
    values[2] = rand() % 50; 
    if(flag) {
        send(centralSocket, &values, 3 * sizeof(int), 0);
        close(centralSocket);
    }
}

void *read_data_distributed(void *params){
    printf("ENTREI PARA LER");
    while(1) {
        clienteLength = sizeof(struct sockaddr_in); 
        printf("PASSOU AQUI");
        clientSock = accept(sock, (struct sockaddr*)&client, (socklen_t*)&clienteLength); 
        handleTCPClient(clientSock);
        close(clientSock);
    }
    printf("DESISTO!");
    close(sock);
}

void handleTCPClient(int socketCliente) {
	int buffer[4];
	int read_size;

    while((read_size = recv(clientSock, &buffer, 2 * sizeof(int), 0) > 0)) {
        printf("TENTOU LER");
        if(read_size < 0) { 
            puts("recv failed"); 
        } 
        for (i = 0; i < 2; i++) { 
            printf("\n%d\n", buffer[i]); 
        } 
    }
}
