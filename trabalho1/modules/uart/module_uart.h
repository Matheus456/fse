#include "../../main.h"
#define RASP_UART_DEVICE                              "/dev/serial0"
#define UART_TEMP 161
#define UART_POT 162
#define UART_PORT "/dev/serial0"

void *uart_main(void *params);
int conexao_uart();
void escrever_dados_uart(unsigned char *, int);
void mensagem_solicitacao(unsigned char *, int);
void ler_dados_uart(unsigned char *, int, int, struct Temperatures *temperatures);
