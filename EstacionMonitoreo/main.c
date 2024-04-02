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
    int handle;
    int luz;

    if(initDevices(&handle)){
        fprintf(stderr, "Error al inicializar dispositivos\n");
        return 1;
    }
    // Capturar señal ctrl+c
    signal(SIGINT, CtrlC_Interrupt);

    // Inicio del programa
    while (!shouldExit) {
        getluminityValues(handle, &luz);

        printf("Luminosidad: %d lux\n", luz);

        // Espera
        usleep(500000);
    }

    // Fin programa
    i2cClose(handle);
    gpioTerminate();

    return 0;
}