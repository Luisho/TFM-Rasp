#include <stdio.h>
#include <pigpio.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#include "SensorsManager.h"

volatile sig_atomic_t  shouldExit = false;


void CtrlC_Interrupt(int signum) {
    shouldExit = true;
}

int main() {
    int handleLum, handleTemp;
    int luz, ambientTemp, objectTemp;

    if(initDevices(&handleLum, &handleTemp)){
        fprintf(stderr, "Error al inicializar dispositivos\n");
        return 1;
    }
    // Capturar señal ctrl+c
    signal(SIGINT, CtrlC_Interrupt);

    // Inicio del programa
    while (!shouldExit) {
        getluminityValues(handle, &luz);
        getAmbientTemperature(handleTemp, &ambientTemp);
        getObjectTemperature(handleTemp, &objectTemp);

        printf("Luminosidad: %d lux | Temperatura ambiente: %.2f °C | Temperatura objeto: %.2f °C\n", luz, ambientTemp, objectTemp);

        // Espera
        usleep(500000);
    }

    // Fin programa
    i2cClose(handle);
    gpioTerminate();

    return 0;
}