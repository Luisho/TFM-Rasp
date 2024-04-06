#include <stdio.h>
#include <pigpio.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#include "SensorsManager.h"
#include "CommunicationModule.h"
#include "dictionary.h"

volatile sig_atomic_t  shouldExit = false;


void CtrlC_Interrupt(int signum) {
    shouldExit = true;
}

int main() {
    int handleLum, handleTemp, spi_handle;
    int luz, valorGases;
    float ambientTemp, objectTemp;

    if(initDevices(&handleLum, &handleTemp)){
        fprintf(stderr, "Error al inicializar dispositivos\n");
        return 1;
    }

    if (Inicializar_SPI(&spi_handle)) {
        fprintf(stderr, "Error al inicializar canal SPI\n");
        return 1;
    }
    iniciarMosquitto();

    // Capturar señal ctrl+c
    signal(SIGINT, CtrlC_Interrupt);

    // Inicio del programa
    while (!shouldExit) {
        getluminityValues(handleLum, &luz);
        getAmbientTemperature(handleTemp, &ambientTemp);
        getObjectTemperature(handleTemp, &objectTemp);

        valorGases = LeerCanalSPI(GASES_CHANNEL, &spi_handle);

        printf("Luminosidad: %d lux | Temperatura ambiente: %.2f °C | Temperatura objeto: %.2f °C | Gases: %d\n", luz, ambientTemp, objectTemp, valorGases);

        publicLum(luz);
        publicAmbTemp(ambientTemp);
        publicObjTemp(objectTemp);
        publicGas(valorGases);
        // Espera
        usleep(200000);
    }

    // Fin programa
    i2cClose(handleLum);
    i2cClose(handleTemp);
    gpioTerminate();

    return 0;
}