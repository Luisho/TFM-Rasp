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
bool shouldExit = false;
int ledState = 0;
int emerStop = 0;

void CtrlC_Interrupt(int signum) {
    shouldExit = true;
}

void buttonPressed(int gpio, int level, uint32_t tick)
{
    if (gpio == BUTTON_GPIO && level == 0)
    {
        printf("Botón de emergencia presionado.\n");
        // Invierte el estado del LED (encendido o apagado)
        ledState = !ledState;
        emerStop = !emerStop;

        // Controla el LED en función del estado actual
        gpioWrite(LED_RED, ledState);
    }
}

int main() {

    if (gpioInitialise() < 0){
        fprintf(stderr, "Error al inicializar GPIO. Asegúrate de ejecutar el programa como superusuario.\n");
        return 1;
    }

    // Captura la señal Ctrl-C
    signal(SIGINT, CtrlC_Interrupt);

    initDevices();
    gpioSetAlertFunc(BUTTON_GPIO, buttonPressed);

    // Inicializar el canal SPI
    spi_handle = Inicializar_SPI();
    if (spi_handle < 0) return 1;

    // variables locales
    int volante, speed;

    // Inicio del programa
    while (!shouldExit) {
        if(emerStop == 0){
            volante = LeerCanalSPI(POT_DESLIZANTE);
            speed = LeerCanalSPI(POT_ROTATORIO);

            movimientos(volante);
            velocidad(speed);
        }
        // Ajustar la espera
        usleep(500000);
    }

    // Fin programa
    Cerrar_SPI();

    return 0;
}
