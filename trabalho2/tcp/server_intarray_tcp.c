
// Server code in C to sort the array 
#include <arpa/inet.h> 
#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h> 

// Array type
#define CONTROLE_I2C 0
#define CONTROLE_GPIO 1

// Temperatura e umidade
#define TEMPERATURA 1
#define UMIDADE 2

// Lâmpadas
#define L_COZINHA 1
#define L_SALA 2
#define L_QUARTO_1 3
#define L_QUARTO_2 4

// Ar-Condicionado
#define AR_QUARTO_1 5
#define AR_QUARTO_2 6

// Sensores de presença
#define SP_SALA 7
#define SP_COZINHA 8

// Sensores de abertura
#define SA_PORTA_COZINHA 9
#define SA_JANELA_COZINHA 10
#define SA_PORTA_SALA 11
#define SA_JANELA_SALA 12
#define SA_JANELA_QUARTO_1 13
#define SA_JANELA_QUARTO_2 14
  
// Driver code 
int main(int argc, char* argv[]) 
{ 
    int socket_desc, client_sock, c, read_size; 
    struct sockaddr_in server, client;
    int message[14], i; 
  
    // Create socket 
    socket_desc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (socket_desc == -1) { 
        printf("Could not create socket"); 
    } 
    puts("Socket created"); 
  
    // Prepare the sockaddr_in structure 
    server.sin_family = AF_INET; 
    server.sin_addr.s_addr = INADDR_ANY; 
    server.sin_port = htons(10001); 
  
    // Bind the socket 
    if (bind(socket_desc, (struct sockaddr*)&server, sizeof(server)) < 0) { 
  
        // print the error message 
        perror("bind failed. Error"); 
        return 1; 
    } 
    puts("bind done"); 
  
    // lsiten to the socket 
    listen(socket_desc, 3); 
  
    puts("Waiting for incoming connections..."); 
    c = sizeof(struct sockaddr_in); 
  
    // accept connection from an incoming client 
    client_sock = accept(socket_desc, (struct sockaddr*)&client, (socklen_t*)&c); 
    
    if (client_sock < 0) { 
        perror("accept failed"); 
        return 1; 
    } 
  
    // Receive a message from client 
    while ((read_size = recv(client_sock, &message, 14 * sizeof(int), 0)) > 0) { 
        // printf("TESTE1");
        printf("\nVALOR: %d\n", message[0]);
        for(int k=0; k<14; k++) {
            printf("%d: %d\n", k, message[k]);
        }
        int dataClient[2] = {10,20};
        send(client_sock, &dataClient, 2 * sizeof(int), 0);
        sleep(5);
    } 
  
    if (read_size == 0) { 
        puts("Client disconnected"); 
    } 
    else if (read_size == -1) { 
        perror("recv failed"); 
    } 
  
    return 0; 
} 
  