#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>

#include "dictionary.h"

int initDevices(int *handleLum){
    // Inicializar librería pigpio
    if (gpioInitialise() < 0) {
        fprintf(stderr, "Error al inicializar pigpio\n");
        return 1;
    }

    //Inicializar bus I2C
    if ((*handleLum = i2cOpen(1, GY30_ADDRESS, 0)) < 0) {
        fprintf(stderr, "Error al abrir el bus I2C del sensor de luminosidad\n");
        gpioTerminate();
        return 1;
    }

    return 0;
}

void getluminityValues(int handleLum, int *lum){
    // Leer el valor de la intensidad de luz del sensor GY-30
     if (i2cWriteByte(handleLum, 0x10) != 0) {
        printf("Error al enviar el comando de inicio de medición\n");
        return;
    }
    time_sleep(0.5);

    uint16_t data = i2cReadWordData(handleLum, 0x00);
    if (data < 0) {
        printf("Error al leer datos del sensor\n");
        return;
    }

    *lum = data / 1.2;
}