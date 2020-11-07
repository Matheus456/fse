
#define QNT_DEVICES 18
#define QNT_OUTPUTS 6
#define TOTAL_DEVICES 14

// Lamps
#define L_KITCHEN 0
#define L_ROOM 1
#define L_BEDROOM_1 2
#define L_BEDROOM_2 3

// Air conditioning
#define AIR_BEDROOM_1 4
#define AIR_BEDROOM_2 5

// Presence sensors
#define SP_ROOM 6
#define SP_KITCHEN 7

// Aperture sensors
#define SA_DOOR_KITCHEN 8
#define SA_WINDOW_KITCHEN 9
#define SA_DOOR_ROOM 10
#define SA_WINDOW_ROOM 11
#define SA_WINDOW_BEDROOM_1 12
#define SA_WINDOW_BEDROOM_2 13

// Temperature e humidity
#define TEMPERATURE 14
#define HUMIDITY 15
#define TEMPERATURE_CONTROL 16

struct devices
{
    int lampKitchen;
    int lampRoom;
    int lampBedroom1;
    int lampBedroom2;
    int airConditioning1;
    int airConditioning2;
};

struct sensors
{
    // presence sensor
    int spRoom;
    int spKitchen;

    // aperture sensor
    int saDoorKitchen;
    int saWindowKitchen;
    int saDoorRoom;
    int saWindowRoom;
    int saWindowBedroom1;
    int saWindowBedroom2;
};

struct climate
{
    double temperature;
    double humidity;
};




struct data
{
    struct climate *climate;
    struct sensors *sensors;
    struct devices *devices;
    double expectedTemperature;
};
