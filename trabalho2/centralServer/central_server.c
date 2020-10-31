// Server code in C to sort the array 
#include <arpa/inet.h> 
#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include <pthread.h>
#include <stdint.h>
#include "central_server.h"
#include "module_menu.h"
#include "utils.h"

// Global variables
int serverSocket, clientSock, distributedSocket, clienteLength; 
struct sockaddr_in server, client, distributedServer;
int message[QNT_DEVICES], i; 
pthread_t trecive, tMenu;
struct climate climate;
struct sensors sensors;
struct devices devices;
struct data data;
  
// Functions
void send_data(int code, int value);
void *read_data(void *params);
void handleTCPClient(int socketCliente, struct data *data, int **mapping);
double int_to_double(int integer, int decimal);

int main(int argc, char* argv[]) 
{ 
    data.climate = &climate;
    data.sensors = &sensors;
    data.devices = &devices;

    pthread_create(&trecive, NULL, read_data, &data);
    pthread_create(&tMenu, NULL, menu, &data);
    send_data(3, 1);
    send_data(3, 1);
    send_data(3, 1);
    send_data(3, 1);
    send_data(3, 1);
    pthread_join (trecive, NULL);
    // pthread_join (tMenu, NULL);

    return 0; 
} 
  

void send_data(int code, int value){
    int flag = initSocketClient(&distributedSocket, &distributedServer, IP, 10101); 
    if(flag) {
        int dataClient[2] = {code, value};
        send(distributedSocket, &dataClient, 2 * sizeof(int), 0);
        close(distributedSocket);
    }
}

void *read_data(void *params){
    struct data *data = params;
    data->sensors->spRoom = 0;

    int *mapping[TOTAL_DEVICES_SENSORS] = {&data->devices->lampKitchen, &data->devices->lampRoom, &data->devices->lampBedroom1,
                    &data->devices->lampBedroom2, &data->devices->airConditioning1, &data->devices->airConditioning2,
                    &data->sensors->spRoom, &data->sensors->spKitchen, &data->sensors->saDoorKitchen, 
                    &data->sensors->saWindowKitchen, &data->sensors->saDoorRoom, &data->sensors->saWindowRoom, 
                    &data->sensors->saWindowBedroom1, &data->sensors->saWindowBedroom2};

    initServerSocket(&serverSocket, &server, IP, 10001, &clienteLength);
    while(1) {
        clienteLength = sizeof(struct sockaddr_in); 
        clientSock = accept(serverSocket, (struct sockaddr*)&client, (socklen_t*)&clienteLength); 
        handleTCPClient(clientSock, data, mapping);
        close(clientSock);
    }
    close(serverSocket);
}

void handleTCPClient(int socketCliente, struct data *data, int **mapping) {
	int buffer[3];
	int read_size;

    while ((read_size = recv(clientSock, &buffer, 3*sizeof(int), 0)) > 0) { 
        for(int k=0; k<3; k++) {
        }
        if(buffer[0] <= 13) {
            *mapping[buffer[0]] = buffer[1];
        }
        else {
            double value = int_to_double(buffer[1], buffer[2]);
            if(buffer[0] == 14) {
                data->climate->temperature = value;
            }
            else {
                data->climate->humidity = value;
            }
        }
    } 
}

double int_to_double(int integer, int decimal){
    double value = integer;
    return value + (double)decimal/10;
}
