#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <bcm2835.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>

#include "control_lcd.h"
#include "./modules/i2c/temperature_module_i2c.h"
#include "./modules/uart/module_uart.h"
#include "./modules/i2c/lcd_module_i2c.h"
#include "./modules/gpio/module_gpio.h"
#include "./modules/csv/module_csv.h"
#include "./modules/menu/module_menu.h"

void trata_interrupcao(int sinal) {
    start_resistor(OFF);
    start_fan(OFF);
    bcm2835_close();
    endwin();
    exit(0);
}


int main(void) {
  pthread_t tI2c, tUart, tLcdPrint, tGpio, tCsv, tMenu;
  struct Temperatures temperatures;
  signal(SIGINT, trata_interrupcao);

  temperatures.tr = 35;
  temperatures.hysteresis = 5;
  strcpy(temperatures.inputType, "t");    
  
  // External Temperature
  pthread_create(&tI2c, NULL, temperature_i2c, &temperatures);
  
  // Intern Temperature and Potentiometer***
  pthread_create(&tUart, NULL, uart_main, &temperatures);
  
  // Lcd
  pthread_create(&tLcdPrint, NULL, lcd_i2c, &temperatures);

  // GPIO
  pthread_create(&tGpio, NULL, temperature_control_gpio, &temperatures);

  // Csv
  pthread_create(&tCsv, NULL, register_csv, &temperatures);

  // Menu
  pthread_create(&tMenu, NULL, menu, &temperatures);

	pthread_join(tI2c, NULL);
	pthread_join(tUart, NULL);
	pthread_join(tLcdPrint, NULL);
	pthread_join(tGpio, NULL);
	pthread_join(tMenu, NULL);
  return 0;
}
