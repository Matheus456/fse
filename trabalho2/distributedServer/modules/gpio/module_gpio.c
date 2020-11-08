#include <stdio.h>
#include <stdlib.h>
#include <bcm2835.h>
#include <sched.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>
#include "sockets.h"
#include "distributed_server.h"
#include "module_gpio.h"

void *polling(void *params);
void temperature_control_gpio(struct climate *climate);
void turn_on_component(int index);
void turn_off_component(int index);
void config_pins();

RPiGPIOPin sensores[QNT_SENSORS] = {PIN_SP_ROOM, PIN_SP_KITCHEN, PIN_SA_DOOR_KITCHEN, PIN_SA_WINDOW_KITCHEN, PIN_SA_DOOR_ROOM, PIN_SA_WINDOW_ROOM, PIN_SA_WINDOW_BEDROOM_1, PIN_SA_WINDOW_BEDROOM_2};
RPiGPIOPin outputs[QNTD_OUTPUTS] = {PIN_L_KITCHEN, PIN_L_ROOM, PIN_L_BEDROOM_1, PIN_L_BEDROOM_2, PIN_AIR_BEDROOM_1, PIN_AIR_BEDROOM_2};
pthread_t tTempControl;

void initGpio(){
    if (!bcm2835_init())
        printf("ERROR");
    config_pins();
}

void *polling(void *params) {
    int *index = (int *)params;
    RPiGPIOPin sensor = sensores[*index];
    bcm2835_gpio_fsel(sensor, BCM2835_GPIO_FSEL_INPT);

    int i;
    while (1) {
        sleep(1);
        while (1 == bcm2835_gpio_lev(sensor)){
            sleep(1);
        };
        send_data(*index+QNTD_OUTPUTS, OFF, 0);
        while (0 == bcm2835_gpio_lev(sensor)){
            sleep(1);
        };
        for (i = 0; i < 5000; i++) {
            if (0 == bcm2835_gpio_lev(sensor)) 
                break;
        }
        send_data(*index+QNTD_OUTPUTS, ON, 0);
        fflush(stdout);
        usleep(500);
    }

    return (EXIT_SUCCESS);
}

void config_pins(){
    // Config output pins
    bcm2835_gpio_fsel(PIN_L_KITCHEN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN_L_ROOM, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN_L_BEDROOM_1, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN_L_BEDROOM_2, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN_AIR_BEDROOM_1, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN_AIR_BEDROOM_2, BCM2835_GPIO_FSEL_OUTP);
}

void initialize_components() {
    bcm2835_gpio_write(PIN_SP_ROOM, OFF);
    bcm2835_gpio_write(PIN_SP_KITCHEN, OFF);
    bcm2835_gpio_write(PIN_SA_DOOR_KITCHEN, OFF);
    bcm2835_gpio_write(PIN_SA_WINDOW_KITCHEN, OFF);
    bcm2835_gpio_write(PIN_SA_WINDOW_KITCHEN, OFF);
    bcm2835_gpio_write(PIN_SA_DOOR_ROOM, OFF);
    bcm2835_gpio_write(PIN_SA_WINDOW_ROOM, OFF);
    bcm2835_gpio_write(PIN_SA_WINDOW_BEDROOM_1, OFF);
    bcm2835_gpio_write(PIN_SA_WINDOW_BEDROOM_2, OFF);
}

void turn_off_component(int index) {
    bcm2835_gpio_write(outputs[index], OFF);
    send_data(index, OFF, 0);
}

void turn_on_component(int index) {
    bcm2835_gpio_write(outputs[index], ON);
    send_data(index, ON, 0);
}

void turn_off_temperature_control(struct climate *climate){
        turn_off_component(AIR_BEDROOM_1);
        turn_off_component(AIR_BEDROOM_2);
        send_data(AIR_BEDROOM_1, OFF, 0);
        usleep(200);
        send_data(AIR_BEDROOM_2, OFF, 0);
        usleep(200);
        send_data(TEMPERATURE_CONTROL, -1, 0);
        climate->expected_temperature = -2;
}

void temperature_control_gpio(struct climate *climate) {
    if(climate->temperature <= climate->expected_temperature) {
        turn_off_component(AIR_BEDROOM_1);
        send_data(AIR_BEDROOM_1, OFF, 0);
        turn_off_component(AIR_BEDROOM_2);
        send_data(AIR_BEDROOM_2, OFF, 0);
    }
    else {
        turn_on_component(AIR_BEDROOM_1);
        send_data(AIR_BEDROOM_1, ON, 0);
        turn_on_component(AIR_BEDROOM_2);
        send_data(AIR_BEDROOM_2, ON, 0);
    }
}
