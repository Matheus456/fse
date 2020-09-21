# requires RPi_I2C_driver.py
import RPi_I2C_driver
from time import *
import smbus2
import bme280

mylcd = RPi_I2C_driver.lcd()
porta_i2c = 1
endereco = 0x76
bus = smbus2.SMBus(porta_i2c)
calibracao_parametros = bme280.load_calibration_params(bus, endereco)

while True:
    dado = bme280.sample(bus, endereco, calibracao_parametros)

    # test 2
    mylcd.lcd_display_string("<MR> 'T:'" + str(float("{: .2f}".format(
        dado.temperature))), 1)
    mylcd.lcd_display_string('U:' + str(float("{:.2f}".format(dado.humidity)))
                             + 'P:' + str(float("{:.2f}".format(dado.pressure))), 2)

    sleep(1)  # 2 sec delay

    mylcd.lcd_clear()
mylcd.backlight(0)
