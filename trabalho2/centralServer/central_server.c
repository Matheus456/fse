// Server code in C to sort the array 
#include <arpa/inet.h> 
#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include <pthread.h>
#include <stdint.h>
#include "central_server.h"
#include "menu.h"
#include "sockets.h"

int main(int argc, char* argv[]) 
{ 
    struct climate climate;
    memset(&climate, 0, sizeof(climate));
    struct sensors sensors;
    memset(&sensors, 0, sizeof(sensors));
    struct devices devices;
    memset(&devices, 0, sizeof(devices));

    pthread_t trecive, tMenu;
    struct data data;
    data.climate = &climate;
    data.sensors = &sensors;
    data.devices = &devices;

    pthread_create(&trecive, NULL, read_data, &data);
    pthread_create(&tMenu, NULL, menu, &data);
    // while(1) {
    //     send_data(0, 1, 0);
    //     send_data(1, 1, 0);
    //     send_data(2, 1, 0);
    //     send_data(3, 1, 0);
    //     send_data(4, 1, 0);
    //     send_data(5, 1, 0);
    //     sleep(1);
    //     send_data(0, 0, 0);
    //     send_data(1, 0, 0);
    //     send_data(2, 0, 0);
    //     send_data(3, 0, 0);
    //     send_data(4, 0, 0);
    //     send_data(5, 0, 0);
    //     sleep(1);
    // }
    pthread_join (trecive, NULL);
    // pthread_join (tMenu, NULL);

    return 0; 
} 
  
