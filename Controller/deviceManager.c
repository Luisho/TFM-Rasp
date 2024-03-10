#include <stdio.h>
#include <pigpio.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>

#include "dictionary.h"

extern int spi_handle;

void initDevices(){
    gpioSetMode(BUTTON_GPIO, PI_INPUT);
    gpioSetPullUpDown(BUTTON_GPIO, PI_PUD_UP);
    gpioSetMode(LED_RED, PI_OUTPUT);
    gpioSetMode(PIN_SENSOR, PI_INPUT);
}

int Inicializar_SPI() {
    if (gpioInitialise() < 0) {
        fprintf(stderr, "No se pudo inicializar Pigpio\n");
        return -1;
    }

    // Inicializar la interfaz SPI
    int spi_handle = spiOpen(SPI_CHANNEL, SPI_SPEED, SPI_FLAGS);
    if (spi_handle < 0) {
        fprintf(stderr, "No se pudo abrir la interfaz SPI\n");
        gpioTerminate();
        return -1;
    }

    return spi_handle;
}

int LeerCanalSPI(int channel) {
    if (channel < 0 || channel > 7) {
        fprintf(stderr, "Número de canal SPI no válido\n");
        return -1;
    }

    char tx_data[3] = {1, 128 + (channel << 4), 0};
    char rx_data[3];

    int result = spiXfer(spi_handle, tx_data, rx_data, 3);
    if (result < 0) {
        fprintf(stderr, "Error al leer el canal %d\n", channel);
        return -1;
    }

    int value = ((rx_data[1] & 3) << 8) + rx_data[2];
    return value;
}

int leerInfrarrojos(){
    if(gpioRead(PIN_SENSOR)==0)
        return 1;
    return 0;
}

void buttonPressed(int gpio, int level, uint32_t tick){
    //Utilizado para evitar dobles pulsaciones
    uint32_t diff = tick - lastButtonPress;

    if (gpio == BUTTON_GPIO && level == 0 && diff > 500000)
    {
        // Invierte el estado del LED (encendido o apagado)
        ledState = !ledState;// Lo quitaré en un futuro

        // Controla el LED en función del estado actual
        gpioWrite(LED_RED, ledState);// Lo quitaré en un futuro
        enviarBoton();

        //se actualiza la variable global
        lastButtonPress = tick;
    }
}

void Cerrar_SPI() {
    spiClose(spi_handle);
    gpioTerminate();
}