
#define QNT_DEVICES 18

// Lâmpadas
#define L_COZINHA 0
#define L_SALA 1
#define L_QUARTO_1 2
#define L_QUARTO_2 3

// Ar-Condicionado
#define AR_QUARTO_1 4
#define AR_QUARTO_2 5

// Sensores de presença
#define SP_SALA 6
#define SP_COZINHA 7

// Sensores de abertura
#define SA_PORTA_COZINHA 8
#define SA_JANELA_COZINHA 9
#define SA_PORTA_SALA 10
#define SA_JANELA_SALA 11
#define SA_JANELA_QUARTO_1 12
#define SA_JANELA_QUARTO_2 13

// Temperatura e umidade
#define TEMPERATURA_INTEIRA 14
#define TEMPERATURA_DECIMAL 15
#define UMIDADE_INTEIRA 16
#define UMIDADE_DECIMAL 17

// Address
#define IP "127.0.0.1"

void send_data(int code, double value);