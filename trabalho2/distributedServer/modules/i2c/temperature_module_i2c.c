// use like
// include linux_userspace.c

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

/******************************************************************************/
/*!                         System header files                               */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>

/******************************************************************************/
/*!                         Own header files                                  */
#include "bme280.h"
#include "bme280_defs.h"
#include "distributed_server.h"
#include "temperature_module_i2c.h"
#include "sockets.h"

void user_delay_us(uint32_t period, void *intf_ptr);
int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);

struct bme280_dev device;
struct identifier id;

void setup_i2c(){

    /* Variable to define the result */
    int8_t result = BME280_OK;
    
    id.fd = open(RASP_I2C_DEVICE, O_RDWR);
    id.dev_addr = BME280_I2C_ADDR_PRIM; //i2c Address
    device.intf = BME280_I2C_INTF;
    device.read = user_i2c_read;
    device.write = user_i2c_write;
    device.delay_us = user_delay_us;

    ioctl(id.fd, I2C_SLAVE, id.dev_addr);
    
    /* Update interface pointer with the structure that contains both device address and file descriptor */
    device.intf_ptr = &id;

    /* Initialize the bme280 */
    result = bme280_init(&device);
}

/*!
 * @brief This function reading the sensor's registers through I2C bus.
 */
int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr)
{
    struct identifier id;
    id = *((struct identifier *)intf_ptr);
    write(id.fd, &reg_addr, 1);
    read(id.fd, data, len);
    return 0;
}

/*!
 * @brief This function provides the delay for required time (Microseconds) as per the input provided in some of the
 * APIs
 */
void user_delay_us(uint32_t period, void *intf_ptr)
{
    usleep(period);
}

/*!
 * @brief This function for writing the sensor's registers through I2C bus.
 */
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr)
{
    uint8_t *buf;
    struct identifier id;

    id = *((struct identifier *)intf_ptr);

    buf = malloc(len + 1);
    buf[0] = reg_addr;
    memcpy(buf + 1, data, len);
    if (write(id.fd, buf, len + 1) < (uint16_t)len)
    {
        return BME280_E_COMM_FAIL;
    }

    free(buf);

    return BME280_OK;
}

/*!
 * @brief This API reads the sensor temperature, pressure and humidity data in forced mode.
 */
void send_temperature_and_humidity(struct climate *climate)
{
    /* Variable to define the result */
    int8_t result = BME280_OK;

    /* Variable to define the selecting sensors */
    uint8_t settings_sel = 0;

    /* Variable to store minimum wait time between consecutive measurement in force mode */
    uint32_t req_delay;

    /* Structure to get the pressure, temperature and humidity values */
    struct bme280_data comp_data;

    /* Recommended mode of operation: Indoor navigation */
    device.settings.osr_h = BME280_OVERSAMPLING_1X;
    device.settings.osr_p = BME280_OVERSAMPLING_16X;
    device.settings.osr_t = BME280_OVERSAMPLING_2X;
    device.settings.filter = BME280_FILTER_COEFF_16;

    settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;

    /* Set the sensor settings */
    result = bme280_set_sensor_settings(settings_sel, &device);

    /*Calculate the minimum delay required between consecutive measurement based upon the sensor enabled
     *  and the oversampling configuration. */
    req_delay = bme280_cal_meas_delay(&device.settings);

    /* Continuously stream sensor data */
    /* Set the sensor to forced mode */
    result = bme280_set_sensor_mode(BME280_FORCED_MODE, &device);

    /* Wait for the measurement to complete and print data */
    device.delay_us(req_delay, device.intf_ptr);
    result = bme280_get_sensor_data(BME280_ALL, &comp_data, &device);

    int integerTemp = (int)comp_data.temperature;
    int decimalTemp = (int)((comp_data.temperature - integerTemp)*100);
    send_data(TEMPERATURE, integerTemp, decimalTemp);
    climate->temperature = comp_data.temperature;
    int integerHum = (int)comp_data.humidity;
    int decimalHum = (int)((comp_data.humidity - integerHum)*100);
    send_data(HUMIDITY, integerHum, decimalHum);
    climate->humidity = comp_data.humidity;
    printf("TEMPERATURA = %lf, HUMIDADE = %lf", comp_data.temperature, comp_data.humidity);
}
