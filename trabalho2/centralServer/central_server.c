#include <stdlib.h>
#include <arpa/inet.h> 
#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include <pthread.h>
#include <ncurses.h>
#include <stdint.h>
#include <signal.h>
#include "central_server.h"
#include "menu.h"
#include "sockets.h"

pthread_t trecive, tMenu;
void handle_interruption(int signal);
int main(int argc, char* argv[]) 
{ 
    signal(SIGINT, handle_interruption);

    struct climate climate;
    memset(&climate, 0, sizeof(climate));
    struct sensors sensors;
    memset(&sensors, 0, sizeof(sensors));
    struct devices devices;
    memset(&devices, 0, sizeof(devices));

    struct data data;
    data.climate = &climate;
    data.sensors = &sensors;
    data.devices = &devices;
    data.expectedTemperature = -1;

    pthread_create(&trecive, NULL, read_data, &data);
    pthread_create(&tMenu, NULL, menu, &data);
    pthread_join (trecive, NULL);

    return 0; 
} 
  
void handle_interruption(int signal){
    handle_close_sockets();
    endwin();
    pthread_cancel(trecive);
    pthread_cancel(tMenu);
    exit(0);
}