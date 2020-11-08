#include <stdlib.h>
#include "central_server.h"
#include <alarm.h>

void turn_on_alarm(){
    system("omxplayer audio.mp3 > log_omxplayer.txt");
}

void turn_down_alarm(struct data *data){
    if(data->sensors->saDoorKitchen == 0 & data->sensors->saDoorRoom == 0 & 
        data->sensors->saWindowBedroom1 == 0 & data->sensors->saWindowBedroom2 == 0 &
        data->sensors->saWindowKitchen == 0 & data->sensors->saWindowRoom == 0 &
        data->sensors->spKitchen == 0 & data->sensors->spRoom == 0) {
        system("pkill omxplayer");
    } 
}
