#include <stdlib.h>
#include <alarm.h>
#include <central_server.h>

int ligado = 0;
void turn_on_alarm(struct data *data){
    if(ligado = 0) {
        system("omxplayer audio.mp3-");
    }
    ligado = 1;
}

void turn_down_alarm(struct data *data){
    if(data->sensors->saDoorKitchen == 0 & data->sensors->saDoorRoom == 0 & 
        data->sensors->saWindowBedroom1 == 0 & data->sensors->saWindowBedroom2 == 0 &
        data->sensors->saWindowKitchen == 0 & data->sensors->saWindowRoom == 0 &
        data->sensors->spKitchen == 0 & data->sensors->spRoom == 0) {
        system("pkill omxplayer");
        ligado = 0;
    } 
}