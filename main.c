#include <stdio.h>
#include <pigpio.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>

#include "dictionary.h"
#include "deviceManager.h"
#include "robotManager.h"

int spi_handle;

bool shouldExit = false;

void CtrlC_Interrupt(int signum) {
    shouldExit = true;
}

int main() {
    // Captura la señal Ctrl-C
    signal(SIGINT, CtrlC_Interrupt);

    // Inicializar el canal SPI
    spi_handle = Inicializar_SPI();
    if (spi_handle < 0) {
        return 1;
    }
    int volante;
    int speed;

    // Inicio del programa
    while (!shouldExit) {
        volante = LeerCanalSPI(POT_DESLIZANTE);
        speed = LeerCanalSPI(POT_ROTATORIO);

        movimientos(volante);
        velocidad(speed);

        // Ajustar la espera
        usleep(500000);
    }

    // Fin programa
    Cerrar_SPI();

    return 0;
}
