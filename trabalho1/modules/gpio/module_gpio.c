#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <bcm2835.h>
#include <unistd.h>
#include "module_gpio.h"


void configura_pinos(){
    // Configura pinos dos LEDs como saídas
    bcm2835_gpio_fsel(RESISTOR, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(FAN, BCM2835_GPIO_FSEL_OUTP);
}

void start_resistor(int resistor_mask) {
    bcm2835_gpio_write(RESISTOR, resistor_mask & 1 );
}

void start_fan(int fan_mask) {
    bcm2835_gpio_write(FAN, fan_mask & 1 );
}



void *temperature_control_gpio(void *params)
{
    struct Temperatures *temperatures = params;
    if (!bcm2835_init())
      exit(1);
      
    configura_pinos();

    int counter;
    while(1){
        if(temperatures->ti < (temperatures->tr - temperatures->hysteresis)){
            // printf("\nLiga resistor\n");
            start_resistor(ON);
            start_fan(OFF);
        }
        else if(temperatures->ti > (temperatures->tr + temperatures->hysteresis)){
            // printf("\nLiga Ventilador\n");
            start_resistor(OFF);
            start_fan(ON);
        } 
        else {
            // printf("\n Desliga os dois\n");
            start_resistor(OFF);
            start_fan(OFF);
        }
        usleep(500000);
    }
    // printf("Resetando\n");
    // start_resistor(OFF);
    // start_fan(OFF);
    // sleep(2);
    // printf("Inicializando Ventoinha\n");
    // start_fan(ON);
    // sleep(3);
    // printf("Inicializando Resistor\n");
    // start_resistor(ON);
    // sleep(3);
    // printf("Desligando Ventoinha\n");
    // start_fan(OFF);
    // sleep(3);
    // printf("Desligando Resistor\n");
    // start_resistor(OFF);
}

