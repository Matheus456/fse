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

