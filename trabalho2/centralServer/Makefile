central_server: 
	gcc central_server.c ./modules/menu/menu.c ./modules/sockets/sockets.c \
		./modules/csv/csv.c \
 	 	-I . \
 	 	-I ./modules/sockets \
		-I ./modules/menu \
		-I ./modules/csv \
		-lpthread -lncurses -o central_server

clean:
	rm -v central_server



# central_server: 
# 	gcc central_server.c \
# 		./dependencies/bme/bme280.c \
# 		./modules/i2c/temperature_module_i2c.c \
# 		./modules/gpio/module_gpio.c \
# 		./modules/menu/module_menu.c \
# 	 	-I ./dependencies/bme \
# 		-I ./dependencies/lcd -lbcm2835\
# 		-lpthread \
# 		-lncurses \
# 		-lwiringPi \
# 		-o central_server


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
