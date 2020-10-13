#include <stdio.h>
#include <time.h>
#include <bcm2835.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include "control_lcd.h"
#include "./modules/i2c/temperature_module_i2c.h"
#include "./modules/uart/module_uart.h"
#include "./modules/i2c/lcd_module_i2c.h"
#include "./modules/gpio/module_gpio.h"
#include "./modules/csv/module_csv.h"
#include "./modules/menu/module_menu.h"

int main(void) {
  pthread_t tI2c, tUart, tLcdPrint, tGpio, tCsv, tMenu;
  struct Temperatures temperatures;

  float tr;
  float h;
  printf("Temperatura desejada:");
  scanf(" %f", &tr);
  printf("Histerese:");
  scanf(" %f", &h);
  temperatures.tr = tr;
  temperatures.hysteresis = h;
    
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
