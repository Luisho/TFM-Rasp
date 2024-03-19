#include <stdio.h>
#include <pigpio.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#include "dictionary.h"
#include "deviceManager.h"
#include "robotManager.h"

int spi_handle;
volatile sig_atomic_t  shouldExit = false;
int ledState = 1;


void CtrlC_Interrupt(int signum) {
    shouldExit = true;
}

int main() {
    if (gpioInitialise() < 0){
        fprintf(stderr, "Error al inicializar GPIO. Asegúrate de ejecutar el programa como superusuario.\n");
        return 1;
    }
    // Capturar señal ctrl+c
    signal(SIGINT, CtrlC_Interrupt);

    iniciarMosquitto();
    initDevices();
    gpioWrite(LED_RED, ledState); // Lo quitaré en un futuro
    gpioSetAlertFunc(BUTTON_GPIO, buttonPressed);

    // Inicializar el canal SPI
    spi_handle = Inicializar_SPI();
    if (spi_handle < 0) return 1;

    // variables locales
    int volante, speed;

    // Inicio del programa
    while (!shouldExit) {
        
        volante = LeerCanalSPI(POT_ROTATORIO);
        speed = LeerCanalSPI(POT_DESLIZANTE);
        
        if(leerInfrarrojos() == 1)
            marchaAtras();

        movimientos(volante);
        velocidad(speed);

        // Espera
        usleep(500000);
    }

    // Fin programa
    gpioWrite(LED_RED, 0);
    enviarBoton();
    Cerrar_SPI();
    gpioTerminate();

    return 0;
}
