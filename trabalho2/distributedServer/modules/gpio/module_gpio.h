#define ON 1
#define OFF 0

/* 
 * Configuração dos pinos dos LEDs e Botão
*/

#define PINO_L_COZINHA RPI_V2_GPIO_P1_11
#define PINO_L_SALA RPI_V2_GPIO_P1_12
#define PINO_L_QUARTO_1 RPI_V2_GPIO_P1_13
#define PINO_L_QUARTO_2 RPI_V2_GPIO_P1_15

// Ar-Condicionado
#define PINO_AR_QUARTO_1 RPI_V2_GPIO_P1_16
#define PINO_AR_QUARTO_2 RPI_V2_GPIO_P1_18


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

void *temperature_control_gpio(void *params);
void start_resistor(int resistor_mask);
void start_fan(int fan_mask);
