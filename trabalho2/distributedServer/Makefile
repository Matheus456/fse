distributed_server: 
	gcc distributed_server.c utils.c \
		-lpthread -lncurses -o distributed_server

clean:
	rm -v distributed_server



# distributed_server: 
# 	gcc distributed_server.c \
# 		./dependencies/bme/bme280.c \
# 		./modules/i2c/temperature_module_i2c.c \
# 		./modules/gpio/module_gpio.c \
# 		./modules/menu/module_menu.c \
# 	 	-I ./dependencies/bme \
# 		-I ./dependencies/lcd -lbcm2835\
# 		-lpthread \
# 		-lncurses \
# 		-lwiringPi \
# 		-o distributed_server
