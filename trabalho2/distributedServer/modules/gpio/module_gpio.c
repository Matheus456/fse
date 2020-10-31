// Código adaptado do Livro: Raspberry Pi And The IoT In C
// Autor: Harry Fairhead
// Disponível em: https://www.iot-programmer.com/index.php/books/22-raspberry-pi-and-the-iot-in-c/chapters-raspberry-pi-and-the-iot-in-c/55-raspberry-pi-and-the$
// 
#include <stdio.h>
#include <stdlib.h>
#include <bcm2835.h>
#include <sched.h>
#include <pthread.h>
#include <sys/mman.h>

// Sensores de presença
#define PINO_SP_SALA RPI_V2_GPIO_P1_22
#define PINO_SP_COZINHA RPI_V2_GPIO_P1_37

// Sensores de abertura
#define PINO_SA_PORTA_COZINHA RPI_V2_GPIO_P1_29
#define PINO_SA_JANELA_COZINHA RPI_V2_GPIO_P1_31
#define PINO_SA_PORTA_SALA RPI_V2_GPIO_P1_32
#define PINO_SA_JANELA_SALA RPI_V2_GPIO_P1_36
#define PINO_SA_JANELA_QUARTO_1 RPI_V2_GPIO_P1_38
#define PINO_SA_JANELA_QUARTO_2 RPI_V2_GPIO_P1_40

#define QNT_SENSORES 8

void *polling(void *params);

int main(){
    // Define a prioridade do programa / thread como máxima 
    const struct sched_param priority = {1};
    sched_setscheduler(0, SCHED_FIFO, &priority);
    // Trava o processo na memória para evitar SWAP
    // mlockall(MCL_CURRENT | MCL_FUTURE);

    // pthread_t t_sensores[8];
    // RPiGPIOPin sensores[8] = {PINO_SP_SALA, PINO_SP_COZINHA, PINO_SA_PORTA_COZINHA, PINO_SA_JANELA_COZINHA, PINO_SA_PORTA_SALA, PINO_SA_JANELA_SALA, PINO_SA_JANELA_QUARTO_1, PINO_SA_JANELA_QUARTO_2};
    pthread_t t_sensores[QNT_SENSORES];
    RPiGPIOPin sensores[QNT_SENSORES] = {PINO_SP_SALA, PINO_SP_COZINHA, PINO_SA_PORTA_COZINHA, PINO_SA_JANELA_COZINHA, PINO_SA_PORTA_SALA, PINO_SA_JANELA_SALA, PINO_SA_JANELA_QUARTO_1, PINO_SA_JANELA_QUARTO_2};

    for(int i=0; i<QNT_SENSORES; i++) {
        pthread_create(&t_sensores[i], NULL, polling, &sensores[i]);
    }
    for(int i=0; i<QNT_SENSORES; i++) {
        pthread_join (t_sensores[i], NULL);
    }
}

void *polling(void *params) {
    RPiGPIOPin *sensor = (RPiGPIOPin *)params;
    
    if (!bcm2835_init())
        printf("ERROR");
  
    bcm2835_gpio_fsel(*sensor, BCM2835_GPIO_FSEL_INPT);

    volatile int i;
    while (1) {
        while (1 == bcm2835_gpio_lev(*sensor));
        while (0 == bcm2835_gpio_lev(*sensor));
        for (i = 0; i < 5000; i++) {
            if (0 == bcm2835_gpio_lev(*sensor)) 
                break;
        }
        printf("Detectado %d\n\r", i); 
        fflush(stdout);
    }

    return (EXIT_SUCCESS);
}
