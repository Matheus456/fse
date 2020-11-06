#include <arpa/inet.h> 
#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include <pthread.h>
#include <stdint.h>
#include "central_server.h"
#include "sockets.h"
#include "csv.h"

double int_to_double(int integer, int decimal);
int initServerSocket(int *socket_desc, struct sockaddr_in *server, char *ip, int porta, int *clienteLength);
int initSocketClient(int *clientSocket, struct sockaddr_in *server, char *ip, int porta);
void handleTCPClient(int socketCliente, struct data *data, int **mapping, int clientSocket);

int distributedSocket, clientSocket, serverSocket;

void send_data(int code, int value, int decimal){
    struct sockaddr_in distributedServer;
    int flag = initSocketClient(&distributedSocket, &distributedServer, DISTRIBUTED_IP, DISTRIBUTED_PORT); 
    if(flag) {
        int dataClient[3] = {code, value, decimal};
        send(distributedSocket, &dataClient, 3 * sizeof(int), 0);
        add_row_csv(code, value, decimal);
    }
    close(distributedSocket);
}

void *read_data(void *params){
    int serverSocket, clienteLength;
    struct sockaddr_in serverAddr, clientAddr;
    struct data *data = params;
    data->sensors->spRoom = 0;

    int *mapping[TOTAL_DEVICES] = {&data->devices->lampKitchen, &data->devices->lampRoom, &data->devices->lampBedroom1,
                    &data->devices->lampBedroom2, &data->devices->airConditioning1, &data->devices->airConditioning2,
                    &data->sensors->spRoom, &data->sensors->spKitchen, &data->sensors->saDoorKitchen, 
                    &data->sensors->saWindowKitchen, &data->sensors->saDoorRoom, &data->sensors->saWindowRoom, 
                    &data->sensors->saWindowBedroom1, &data->sensors->saWindowBedroom2};

    initServerSocket(&serverSocket, &serverAddr, CENTRAL_IP, CENTRAL_PORT, &clienteLength);
    while(1) {
        clienteLength = sizeof(struct sockaddr_in); 
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, (socklen_t*)&clienteLength); 
        handleTCPClient(clientSocket, data, mapping, clientSocket);
        close(clientSocket);
    }
    close(serverSocket);
}

void handleTCPClient(int socketCliente, struct data *data, int **mapping, int clientSocket) {
	int buffer[3];
	int read_size;

    while ((read_size = recv(clientSocket, &buffer, 3*sizeof(int), 0)) > 0) { 
        if(buffer[0] <= 13) {
            *mapping[buffer[0]] = buffer[1];
        }
        else if(buffer[0] == TEMPERATURE_CONTROL) {
            data->expectedTemperature = -1;
        }
        else {
            // printf("PASSANDO valueES: %d, %d", buffer[1], buffer[2])
            double value = int_to_double(buffer[1], buffer[2]);
            if(buffer[0] == TEMPERATURE) {
                data->climate->temperature = value;
            }
            else if(buffer[0] == HUMIDITY){
                data->climate->humidity = value;
            }
        }
    } 
}

double int_to_double(int integer, int decimal){
    double value = integer;
    return value + (double)decimal/100;
}

int initServerSocket(int *socket_desc, struct sockaddr_in *server, char *ip, int porta, int *clienteLength){
    // Create socket 
    *socket_desc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (*socket_desc == -1) { 
        printf("Could not create socket"); 
    } 
  
    // Prepare the sockaddr_in structure 
    server->sin_family = AF_INET; 
    server->sin_addr.s_addr = inet_addr(ip); 
    server->sin_port = htons(porta); 
  
    // Bind the socket 
    if (bind(*socket_desc, (struct sockaddr*)server, sizeof(*server)) < 0) { 
        return 0;
    } 

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

    // Prepare the sockaddr_in structure 
    memset(server, 0, sizeof(*server));
    server->sin_family = AF_INET; 
    server->sin_addr.s_addr = inet_addr(ip); 
    server->sin_port = htons(porta); 
  
    if (connect(*clientSocket, (struct sockaddr*)server, sizeof(*server)) < 0) { 
        return 0; 
    }  
    else {
        return 1;
    }

}

void handle_close_sockets()
{
    close(distributedSocket);
    close(clientSocket);
    close(serverSocket);
}