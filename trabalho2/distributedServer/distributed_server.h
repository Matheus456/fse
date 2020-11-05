
#define QNT_DEVICES 18
#define QNTD_OUTPUTS 6
#define QNTD_INPUTS 12

// Lâmpadas
#define L_KITCHEN 0
#define L_ROOM 1
#define L_BEDROOM_1 2
#define L_BEDROOM_2 3

// Ar-Condicionado
#define AIR_BEDROOM_1 4
#define AIR_BEDROOM_2 5

// Sensores de presença
#define SP_ROOM 6
#define SP_KITCHEN 7

// Sensores de abertura
#define SA_DOOR_KITCHEN 8
#define SA_WINDOW_KITCHEN 9
#define SA_DOOR_ROOM 10
#define SA_WINDOW_ROOM 11
#define SA_WINDOW_ROOM_1 12
#define SA_WINDOW_ROOM_2 13

// Temperature and Humidity
#define TEMPERATURE 14
#define HUMIDITY 15
#define TEMPERATURE_CONTROL 16

struct climate
{
    double expected_temperature;
    double temperature;
    double humidity;
};
