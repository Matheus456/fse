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
#include "temperature_module_i2c.h"

void user_delay_us(uint32_t period, void *intf_ptr);
void print_sensor_data(struct bme280_data *comp_data, struct Temperatures *temperatures);
int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);
int8_t stream_sensor_data_forced_mode(struct bme280_dev *dev, struct Temperatures *temperatures);

void *temperature_i2c(void *params)
{
    struct Temperatures *temperatures = params;
    struct bme280_dev dev;
    struct identifier id;

    /* Variable to define the result */
    int8_t result = BME280_OK;
    
    if ((id.fd = open(RASP_I2C_DEVICE, O_RDWR)) < 0)
    {
        fprintf(stderr, "Failed to open the i2c bus %s\n", RASP_I2C_DEVICE);
        exit(1);
    }
    
    id.dev_addr = BME280_I2C_ADDR_PRIM; //i2c Address
    dev.intf = BME280_I2C_INTF;
    dev.read = user_i2c_read;
    dev.write = user_i2c_write;
    dev.delay_us = user_delay_us;

    if (ioctl(id.fd, I2C_SLAVE, id.dev_addr) < 0)
    {
        fprintf(stderr, "Failed to acquire bus access and/or talk to slave.\n");
        exit(1);
    }

    /* Update interface pointer with the structure that contains both device address and file descriptor */
    dev.intf_ptr = &id;

    /* Initialize the bme280 */
    result = bme280_init(&dev);
    if (result != BME280_OK)
    {
        fprintf(stderr, "Failed to initialize the device (code %+d).\n", result);
        exit(1);
    }

    result = stream_sensor_data_forced_mode(&dev, temperatures);
    if (result != BME280_OK)
    {
        fprintf(stderr, "Failed to stream sensor data (code %+d).\n", result);
        exit(1);
    }

    return 0;
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
int8_t stream_sensor_data_forced_mode(struct bme280_dev *dev, struct Temperatures *temperatures)
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
    dev->settings.osr_h = BME280_OVERSAMPLING_1X;
    dev->settings.osr_p = BME280_OVERSAMPLING_16X;
    dev->settings.osr_t = BME280_OVERSAMPLING_2X;
    dev->settings.filter = BME280_FILTER_COEFF_16;

    settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;

    /* Set the sensor settings */
    result = bme280_set_sensor_settings(settings_sel, dev);
    if (result != BME280_OK)
    {
        fprintf(stderr, "Failed to set sensor settings (code %+d).", result);

        return result;
    }

    printf("Temperature, Pressure, Humidity\n");

    /*Calculate the minimum delay required between consecutive measurement based upon the sensor enabled
     *  and the oversampling configuration. */
    req_delay = bme280_cal_meas_delay(&dev->settings);

    /* Continuously stream sensor data */
    while (1)
    {
        /* Set the sensor to forced mode */
        result = bme280_set_sensor_mode(BME280_FORCED_MODE, dev);
        if (result != BME280_OK)
        {
            fprintf(stderr, "Failed to set sensor mode (code %+d).", result);
            // break;
        }

        /* Wait for the measurement to complete and print data */
        dev->delay_us(req_delay, dev->intf_ptr);
        result = bme280_get_sensor_data(BME280_ALL, &comp_data, dev);
        if (result != BME280_OK)
        {
            fprintf(stderr, "Failed to get sensor data (code %+d).", result);
            // break;
        }
        temperatures->te = comp_data.temperature;
        // print_sensor_data(&comp_data, temperatures);
        // printf("Temperatura Externa: %f\n", comp_data.temperature);
        // print_sensor_data(&comp_data);
        usleep(500000);
    }

    return result;
}
