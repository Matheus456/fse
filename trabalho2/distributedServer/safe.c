// CENTRAL
// Server code in C to sort the array 
#include <arpa/inet.h> 
#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include <pthread.h>
#include "central_server.h"
#include "module_menu.h"

// Global variables
int serverSocket, clientSock, c, read_size; 
struct sockaddr_in server, client;
int message[QNT_DEVICES], i; 
pthread_t trecive, tMenu;
  
// Functions
void send_data(int code, double value);
void *read_data(void *params);
int main(int argc, char* argv[]) 
{ 
  
    // Create socket 
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (serverSocket == -1) { 
        printf("Could not create socket"); 
    } 
  
    // Prepare the sockaddr_in structure 
    server.sin_family = AF_INET; 
    server.sin_addr.s_addr = INADDR_ANY; 
    server.sin_port = htons(10001); 
  
    // Bind the socket 
    if (bind(serverSocket, (struct sockaddr*)&server, sizeof(server)) < 0) { 
        return 1; 
    } 
  
    // lsiten to the socket 
    listen(serverSocket, 3); 
  
    c = sizeof(struct sockaddr_in); 
  
    // accept connection from an incoming client 
    clientSock = accept(serverSocket, (struct sockaddr*)&client, (socklen_t*)&c); 
    
    if (clientSock < 0) { 
        return 1; 
    } 
  

    pthread_create(&trecive, NULL, read_data, NULL);
    pthread_create(&tMenu, NULL, menu, &message);
    while(1){
        send_data(3, 1);
        sleep(5);
        send_data(1, 36.5);
        sleep(1);
    }
    pthread_join (trecive, NULL);
    pthread_join (tMenu, NULL);


    return 0; 
} 
  
void *read_data(void *params){
    while ((read_size = recv(clientSock, &message, QNT_DEVICES * sizeof(int), 0)) > 0) { 
        printf("\nVALOR: %d\n", message[0]);
        for(int k=0; k<QNT_DEVICES; k++) {
            printf("%d: %d\n", k, message[k]);
        }
        sleep(1);
        message[0] = 20;
        sleep(1);
    } 
}

void send_data(int code, double value){
    double dataClient[2] = {code, value};
    send(clientSock, &dataClient, 2 * sizeof(double), 0);
}





// DISTRIBUTED
// Client code in C to sort the array 
#include <arpa/inet.h> 
#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include <pthread.h>
#include "central_server.h"
  

// Driver code 
int serverSocket, clientSock, c; 
int sock; 
struct sockaddr_in distributed_server, central_server, client; 
double server_reply[2]; 
int devices[QNT_DEVICES] = { 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 36, 5, 20, 7 };
int i, temp; 
pthread_t trecive, tsend;

void *escuta(void *params);
void *envia(void *params);

int main(int argc, char* argv[]) 
{ 
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (sock == -1) { 
        printf("Could not create socket"); 
    } 
  
    distributed_server.sin_addr.s_addr = INADDR_ANY; 
    distributed_server.sin_family = AF_INET; 
    distributed_server.sin_port = htons(10101); 

    // Bind the socket 
    if (bind(sock, (struct sockaddr*)&distributed_server, sizeof(distributed_server)) < 0) { 
        return 1; 
    } 
  
    // listen to the socket 
    listen(serverSocket, 3); 

    c = sizeof(struct sockaddr_in); 
  
    // accept connection from an incoming client 
    clientSock = accept(serverSocket, (struct sockaddr*)&client, (socklen_t*)&c); 
    printf("PASSOU AQUI");

    central_server.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    central_server.sin_family = AF_INET; 
    central_server.sin_port = htons(10001); 
    
    // Connect to remote central_server 
    if (connect(sock, (struct sockaddr*)&central_server, sizeof(central_server)) < 0) { 
        perror("connect failed. Error"); 
        return 1; 
    }
  
    printf("CRIOU THREADS");
    pthread_create(&tsend, NULL, envia, NULL);
    pthread_create(&trecive, NULL, escuta, NULL);
    pthread_join (trecive, NULL);
    pthread_join (tsend, NULL);
    close(sock); 
    return 0; 
} 

void *envia(void *params){
    printf("ENVIA");
    while(1) {
        if (send(sock, &devices, QNT_DEVICES * sizeof(int), 0) < 0) { 
            puts("Send failed"); 
        } 
        sleep(5);
    }
}


void *escuta(void *params){
    printf("Entrou escuta");
    int read_size;
    while((read_size = recv(clientSock, &server_reply, 2 * sizeof(double), 0) > 0)) {
        // printf("ESCUTA\n");
        printf("TENTOU LER");
        if(read_size < 0) { 
            puts("recv failed"); 
        } 
        for (i = 0; i < 2; i++) { 
            printf("\n%lf\n", server_reply[i]); 
        } 
        // sleep(2);
    }
}
