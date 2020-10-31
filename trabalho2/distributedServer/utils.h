int initServerSocket(int *server, struct sockaddr_in *addr, char *ip, int porta, int *clienteLength);
int initSocketClient(int *serverSocket, struct sockaddr_in *server, char *ip, int porta);