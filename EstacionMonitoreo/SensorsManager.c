#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>

#include "dictionary.h"

int initDevices(int *handleLum, int *handleTemp){
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

    if ((*handleTemp = i2cOpen(1, HW691_ADDRESS, 0)) < 0) {
        fprintf(stderr, "Error al abrir el bus I2C del sensor de Temperatura\n");
        gpioTerminate();
        return 1;
    }

    return 0;
}

// Función para inicializar la comunicación SPI
int Inicializar_SPI(int *spi_handle) {
    if (gpioInitialise() < 0) {
        fprintf(stderr, "No se pudo inicializar Pigpio\n");
        return -1;
    }

    // Inicializar la interfaz SPI
    if ((*spi_handle = spiOpen(SPI_CHANNEL, SPI_SPEED, SPI_FLAGS)) < 0) {
        fprintf(stderr, "No se pudo abrir la interfaz SPI\n");
        gpioTerminate();
        return -1;
    }

    return spi_handle;
}

// Función para realizar una lectura analógica utilizando MCP3008
int LeerCanalSPI(const int channel, int *spi_handle) {
    if (channel < 0 || channel > 7) {
        fprintf(stderr, "Número de canal SPI no válido\n");
        return -1;
    }

    char tx_data[3] = {1, 128 + (channel << 4), 0};
    char rx_data[3];

    int result = spiXfer(*spi_handle, tx_data, rx_data, 3);
    if (result < 0) {
        fprintf(stderr, "Error al leer el canal %d\n", channel);
        return -1;
    }

    int value = ((rx_data[1] & 3) << 8) + rx_data[2];
    return value;
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

void getAmbientTemperature(int handleTemp, float *ambientTemp){
    // Leer temperatura ambiente en grados Celsius
    *ambientTemp = i2cReadWordData(handleTemp, AMBIENT_TEMP_CHANNEL) * 0.02 - 273.15;
}

void getObjectTemperature(int handleTemp, float *objectTemp){
    // Leer temperatura del objeto en grados Celsius
    *objectTemp = i2cReadWordData(handleTemp, OBJECT_TEMP_CHANNEL) * 0.02 - 273.15;
}