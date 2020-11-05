
// Client code in C to sort the array 
#include <arpa/inet.h> 
#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include <pthread.h>

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
int sock; 
struct sockaddr_in server; 
int server_reply[2]; 
int devices[14] = { CONTROLE_I2C, 4, 3, 8, 9, 1, 2, 0, 6, 3, 4, 5, 10, 3 }, i, temp; 
pthread_t trecive, tsend;

void *escuta(void *params);
void *envia(void *params);

int main(int argc, char* argv[]) 
{ 
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (sock == -1) { 
        printf("Could not create socket"); 
    } 
  
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    server.sin_family = AF_INET; 
    server.sin_port = htons(8880); 
  
    // Connect to remote server 
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) { 
        perror("connect failed. Error"); 
        return 1; 
    } 
  
    printf("CRIOU THREADS");
    pthread_create(&tsend, NULL, envia, NULL);
    pthread_create(&trecive, NULL, escuta, NULL);
    pthread_join (trecive, NULL);
    pthread_join (tsend, NULL);
    close(sock); 
    return 0; 
} 

void *envia(void *params){
    printf("ENVIA");
    while(1) {
        if (send(sock, &devices, 14 * sizeof(int), 0) < 0) { 
            puts("Send failed"); 
        } 
        sleep(5);
    }
}


void *escuta(void *params){
    printf("Entrou escuta");
    while(1) {
        // printf("ESCUTA\n");
        printf("TENTOU LER");
        if(recv(sock, &server_reply, 2 * sizeof(int), 0) < 0) { 
            puts("recv failed"); 
        } 
        for (i = 0; i < 2; i++) { 
            printf("\n%d\n", server_reply[i]); 
        } 
        // sleep(2);
    }
}
