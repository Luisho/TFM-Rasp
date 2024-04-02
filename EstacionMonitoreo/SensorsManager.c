#include <stdio.h>
#include <pigpio.h>

#include "dictionary.h"

int initDevices(int *handle){
    // Inicializar librería pigpio
    if (gpioInitialise() < 0) {
        fprintf(stderr, "Error al inicializar pigpio\n");
        return 1;
    }

    //Inicializar bus I2C
    if ((*handle = i2cOpen(1, 0x00, 0)) < 0) {
        fprintf(stderr, "Error al abrir el bus I2C\n");
        gpioTerminate();
        return 1;
    }

    return 0;
}

void getI2CValues(const int handle, int *lum, int *temp){
    // Leer el valor de la intensidad de luz del sensor GY-30
    i2cWriteByte(handle, GY30_ADDRESS);
    *lum = i2cReadWordData(handle, 0);

    // Leer el valor de temperatura del sensor HW-691
    i2cWriteByte(handle, HW691_ADDRESS);
    *temp = i2cReadWordData(handle, 0);
}