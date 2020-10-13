teste: 
	gcc main.c ./dependencies/bme/bme280.c \
		./modules/i2c/temperature_module_i2c.c \
		./modules/uart/module_uart.c \
		./modules/i2c/lcd_module_i2c.c \
		./modules/gpio/module_gpio.c \
		./modules/csv/module_csv.c \
		./modules/menu/module_menu.c \
	 	-I ./dependencies/bme \
		-I ./dependencies/lcd -lbcm2835\
		-lpthread \
		-lncurses \
		-lwiringPi \
		-o trabalho2

raspbian: 
	gcc main.c ./dependencies/bme/bme280.c temperature_module_i2c.c \
	 	-I ./dependencies/bme \
		-I ./dependencies/lcd \
		-lwiringPi -o trabalho2

clean:
	rm -v trabalho2	