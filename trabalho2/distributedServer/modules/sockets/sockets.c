
#include <stdlib.h>
#include <arpa/inet.h> 
#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include "sockets.h"
#include "distributed_server.h"
#include "module_gpio.h"

int initServerSocket(int *serverSocket, struct sockaddr_in *server, char *ip, int porta, int *clienteLength);
int initSocketClient(int *serverSocket, struct sockaddr_in *server, char *ip, int porta);
void handleTCPClient(int clientSocket, struct climate *climate);
double int_to_double(int integer, int decimal);

int centralSocket, serverSocket, clientSocket;

void send_data(int code, int value, int decimal){
    struct sockaddr_in centralServerAddr;
    int flag = initSocketClient(&centralSocket, &centralServerAddr, CENTRAL_IP, CENTRAL_PORT); 
    if(flag) {
        int values[3] = {code, value, decimal};
        printf("ENVIADO: %d %d %d\n", code, value, decimal);
        send(centralSocket, &values, 3 * sizeof(int), 0);
    }
    close(centralSocket);
}

void *read_data(void *params){
    struct climate *climate = params;
    struct sockaddr_in distributed_serverAddr, clientAddr; 
    int serverSocket, clienteLength; 
    initServerSocket(&serverSocket, &distributed_serverAddr, DISTRIBUTED_IP, DISTRIBUTED_PORT, &clienteLength);
    while(1) {
        clienteLength = sizeof(struct sockaddr_in); 
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, (socklen_t*)&clienteLength); 
        handleTCPClient(clientSocket, climate);
        close(clientSocket);
    }
    close(serverSocket);
}

void handleTCPClient(int clientSocket, struct climate *climate) {
	int buffer[4];
	int read_size;
    while((read_size = recv(clientSocket, &buffer, 3 * sizeof(int), 0) > 0)) {
        printf("Valores = [%d, %d, %d]\n", buffer[0], buffer[1], buffer[2]);
        if(read_size < 0) { 
            puts("recv failed"); 
        } 
        if(buffer[0] < QNTD_OUTPUTS){
            if(buffer[1] == 1){
                turn_on_component(buffer[0]);
            }
            else {
                turn_off_component(buffer[0]);
            }
        }
        else if(buffer[0] == TEMPERATURE_CONTROL) {
            climate->expected_temperature = int_to_double(buffer[1], buffer[2]);
        }
    }
}

double int_to_double(int integer, int decimal){
    double value = integer;
    return value + (double)decimal/100;
}

int initServerSocket(int *serverSocket, struct sockaddr_in *server, char *ip, int porta, int *clienteLength){
    // Create socket 
    *serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (*serverSocket == -1) { 
        printf("Could not create socket\n"); 
    } 
    puts("Socket Server created\n"); 
  

    // Prepare the sockaddr_in structure 
    memset(server, 0, sizeof(struct sockaddr_in));
    server->sin_family = AF_INET; 
    server->sin_addr.s_addr = htonl(INADDR_ANY);
    server->sin_port = htons(porta); 
  
    // Bind the socket 
    if (bind(*serverSocket, (struct sockaddr*)server, sizeof(struct sockaddr_in)) < 0) { 
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
    *serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (*serverSocket < 0) { 
        printf("Could not create socket\n"); 
        return 0; 
    } 
  
    // Prepare the sockaddr_in structure 
    memset(server, 0, sizeof(struct sockaddr_in));
    server->sin_family = AF_INET; 
    server->sin_addr.s_addr = inet_addr(ip); 
    server->sin_port = htons(porta); 
  
    if (connect(*serverSocket, (struct sockaddr*)server, sizeof(struct sockaddr_in)) < 0) { 
        puts("connect error");
        return 0; 
    }  
    else {
        return 1;
    }
}

void handle_close_sockets()
{
    close(centralSocket);
    close(centralSocket);
    close(serverSocket);
}