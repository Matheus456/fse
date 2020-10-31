#include "bme280_defs.h"
#define RASP_I2C_DEVICE                              "/dev/i2c-1"

/* Structure that contains identifier details used in example */
struct identifier
{
    /* Variable to hold device address */
    uint8_t dev_addr;
    /* Variable that contains file descriptor */
    int8_t fd;
};

void *temperature_i2c(void *params);
