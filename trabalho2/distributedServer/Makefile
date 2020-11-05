distributed_server: 
	gcc distributed_server.c ./dependencies/bme/bme280.c \
		./modules/sockets/sockets.c \
		./modules/gpio/module_gpio.c \
 		./modules/i2c/temperature_module_i2c.c \
 	 	-I . \
 	 	-I ./modules/sockets \
 	 	-I ./modules/gpio \
 	 	-I ./modules/i2c \
 	 	-I ./dependencies/bme \
		-lpthread -lncurses -lbcm2835 -lwiringPi -o distributed_server

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
