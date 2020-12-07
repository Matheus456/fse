
// Client code in C to sort the array 
#include <arpa/inet.h> 
#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include "sockets.h"
#include "distributed_server.h"
#include "module_gpio.h"
#include "temperature_module_i2c.h"

void *read_data_distributed(void *params);
void *send_data_distributed(void *params);
void handleTCPClient(int socketCliente);
void *climate_control(void *params);
void handle_alarm();


pthread_mutex_t set_temperature_mutex;
int main(int argc, char* argv[]) 
{ 
    pthread_t trecive, tsend, ti2c;
    pthread_t t_sensores[QNT_SENSORS];
    signal(SIGALRM, handle_alarm);
    pthread_mutex_init(&set_temperature_mutex, NULL);
    ualarm(2e5, 2e5);
    
    struct climate climate;
    climate.expected_temperature = -2;
    // Creating server
    pthread_create(&trecive, NULL, read_data, &climate);

    // Setup i2c 
    setup_i2c();
    pthread_create(&ti2c, NULL, climate_control, &climate);

    // Creating gpio threads to polling
    initGpio();
    int aux[QNT_SENSORS] = {0,1,2,3,4,5,6,7};
    for(int index=0; index<QNT_SENSORS; index++) {
        pthread_create(&t_sensores[index], NULL, polling, &aux[index]);
    }
    for(int index=0; index<QNT_SENSORS; index++) {
        pthread_join (t_sensores[index], NULL);
    }
    pthread_join (trecive, NULL);
    pthread_join (ti2c, NULL);

    return 0; 
} 

void handle_alarm(){
    pthread_mutex_unlock(&set_temperature_mutex);
}

void *climate_control(void *params) {
    while(1){
        pthread_mutex_lock(&set_temperature_mutex);
        struct climate *climate = params;
        send_temperature_and_humidity(climate);
        if(climate->expected_temperature > 0){
            temperature_control_gpio(climate);
        }
    }
}

