#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int initServerSocket(int *serverSocket, struct sockaddr_in *server, char *ip, int porta, int *clienteLength){
    // Create socket 
    *serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (*serverSocket == -1) { 
        printf("Could not create socket"); 
    } 
    puts("Socket Server created"); 
  
    // Prepare the sockaddr_in structure 
    server->sin_family = AF_INET; 
    server->sin_addr.s_addr = inet_addr(ip); 
    server->sin_port = htons(porta); 
  
    // Bind the socket 
    if (bind(*serverSocket, (struct sockaddr*)server, sizeof(*server)) < 0) { 
        return 0;
    } 

    puts("bind done"); 
  
    // lsiten to the socket 
    listen(*serverSocket, 3); 
    *clienteLength = sizeof(struct sockaddr_in); 
    return 1;
}

int initSocketClient(int *serverSocket, struct sockaddr_in *server, char *ip, int porta){

    // Create socket 
    *serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (*serverSocket == -1) { 
        printf("Could not create socket"); 
        return 0; 
    } 
    puts("Socket Client created"); 
  
    // Prepare the sockaddr_in structure 
    server->sin_family = AF_INET; 
    server->sin_addr.s_addr = inet_addr(ip); 
    server->sin_port = htons(porta); 
  
    if (connect(*serverSocket, (struct sockaddr*)server, sizeof(*server)) < 0) { 
        puts("DEU RUIM"); 
        return 0; 
    }  
    else {
        return 1;
    }

}