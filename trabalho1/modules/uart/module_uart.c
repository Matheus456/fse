#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //Usado para a UART
#include <fcntl.h>   //Usado para a UART
#include <termios.h> //Usado para a UART
#include <string.h>
#include "module_uart.h"

int conexao_uart();
void escrever_dados_uart(unsigned char *, int);
void mensagem_solicitacao(unsigned char *, int);
void ler_dados_uart(unsigned char *, int, int, struct Temperatures *temperatures);

void *uart_main(void *params)
{
    struct Temperatures *temperatures = params;
    int escolha;
    int uart0_filestream = -1;
    uart0_filestream = conexao_uart();
    unsigned char tx_buffer_temp[10];
    unsigned char tx_buffer_pot[10];
    while(1) {
        mensagem_solicitacao(&tx_buffer_temp[0], UART_TEMP);
        mensagem_solicitacao(&tx_buffer_pot[0], UART_POT);

        escrever_dados_uart(&tx_buffer_temp[0], uart0_filestream);
        ler_dados_uart(&tx_buffer_temp[0], uart0_filestream, UART_TEMP, temperatures);
        
        if(!strcmp(temperatures->inputType, "p")) {
            escrever_dados_uart(&tx_buffer_pot[0], uart0_filestream);
            ler_dados_uart(&tx_buffer_pot[0], uart0_filestream, UART_POT, temperatures);
        }
        usleep(500000);
    }
    close(uart0_filestream);
}

int conexao_uart()
{
    int uart0_filestream = -1;
    uart0_filestream = open(UART_PORT, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK); //Open in non blocking read/write mode

    if (uart0_filestream == -1)
    {
        close(uart0_filestream);
    }
    else
    {
        // printf("UART inicializada!\n");
    }

    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B115200 | CS8 | CLOCAL | CREAD; //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);
    return uart0_filestream;
}

void ler_dados_uart(unsigned char *tx_buffer, int uart0_filestream, int codigo, struct Temperatures *temperatures)
{
    float rx_buffer;
    int rx_length = read(uart0_filestream, &rx_buffer, sizeof(rx_buffer)); //Filestream, buffer to store in, number of bytes to read (max)
    if (codigo == UART_TEMP){
        temperatures->ti = rx_buffer;
    } else {
        temperatures->tr = rx_buffer;
    } 
    // printf("PASSOU! Temperatura TI: %f | Temperatura POT: %f ", temperatures->ti, temperatures->tr);
}

void escrever_dados_uart(unsigned char *tx_buffer, int uart0_filestream)
{
    int count = write(uart0_filestream, tx_buffer, strlen((char *)tx_buffer));
}

void mensagem_solicitacao(unsigned char *tx_buffer, int code)
{
    unsigned char *p_tx_buffer;
    p_tx_buffer = tx_buffer;
    *p_tx_buffer++ = code;
    *p_tx_buffer++ = 6;
    *p_tx_buffer++ = 7;
    *p_tx_buffer++ = 2;
    *p_tx_buffer++ = 1;
    *p_tx_buffer++ = '\0';
}