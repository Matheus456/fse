#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int initServerSocket(int *socket_desc, struct sockaddr_in *server, char *ip, int porta, int *clienteLength){
    // Create socket 
    *socket_desc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (*socket_desc == -1) { 
        printf("Could not create socket"); 
    } 
    puts("Socket Server created"); 
  
    // Prepare the sockaddr_in structure 
    server->sin_family = AF_INET; 
    server->sin_addr.s_addr = inet_addr(ip); 
    server->sin_port = htons(porta); 
  
    // Bind the socket 
    if (bind(*socket_desc, (struct sockaddr*)server, sizeof(*server)) < 0) { 
        return 0;
    } 

    puts("bind done"); 
  
    // lsiten to the socket 
    listen(*socket_desc, 3); 
    *clienteLength = sizeof(struct sockaddr_in); 
    return 1;
}

int initSocketClient(int *clientSocket, struct sockaddr_in *server, char *ip, int porta){

    // Create socket 
    *clientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (*clientSocket == -1) { 
        printf("Could not create socket"); 
        return 0; 
    } 
    puts("Socket Client created"); 

    // Prepare the sockaddr_in structure 
    memset(server, 0, sizeof(*server));
    server->sin_family = AF_INET; 
    server->sin_addr.s_addr = inet_addr(ip); 
    server->sin_port = htons(porta); 
  
    if (connect(*clientSocket, (struct sockaddr*)server, sizeof(*server)) < 0) { 
        puts("DEU RUIM");
        return 0; 
    }  
    else {
        return 1;
    }

}