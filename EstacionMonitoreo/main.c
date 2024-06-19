#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#include "SensorsManager.h"
#include "CommunicationModule.h"
#include "dictionary.h"

volatile sig_atomic_t shouldExit = false;
pid_t pythonProcessID = -1; // Almacenará el ID del proceso Python

void CtrlC_Interrupt(int signum) {
    shouldExit = true;
    // Envía una señal SIGINT al proceso Python hijo para detenerlo
    if (pythonProcessID > 0) {
        kill(pythonProcessID, SIGINT);
    }
}

int main() {
    int handleLum, handleTemp, spi_handle;
    int luz, valorGases;
    float ambientTemp, objectTemp;

    // Iniciar dispositivos
    if (initDevices(&handleLum, &handleTemp)) {
        fprintf(stderr, "Error al inicializar dispositivos\n");
        return 1;
    }

    // Iniciar canal SPI
    if (Inicializar_SPI(&spi_handle)) {
        fprintf(stderr, "Error al inicializar canal SPI\n");
        return 1;
    }

    // Iniciar Mosquitto
    iniciarMosquitto();

    // Capturar señal ctrl+c
    signal(SIGINT, CtrlC_Interrupt);

    // Ejecutar el script Python
    char command[100];
    snprintf(command, sizeof(command), "python3 CameraModule/camera.py");
    pythonProcessID = fork();
    if (pythonProcessID == 0) {
        // Proceso hijo: ejecuta el script Python
        system(command);
        exit(0);
    }
    // Inicio del programa en C
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

    // Fin del programa en C
    i2cClose(handleLum);
    i2cClose(handleTemp);
    gpioTerminate();

    return 0;
}
