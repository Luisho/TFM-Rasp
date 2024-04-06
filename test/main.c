#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>

#define SPI_CHANNEL 0
#define SPI_SPEED   1000000  // Velocidad en Hz
#define SPI_FLAGS   0

int spi_handle;

// Función para inicializar la comunicación SPI
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

// Función para realizar una lectura analógica utilizando MCP3008
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

int main() {
    if (gpioInitialise() < 0) {
        fprintf(stderr, "Error al inicializar pigpio\n");
        return 1;
    }

    spi_handle = Inicializar_SPI();
    if (spi_handle < 0) {
        return 1;
    }

    printf("Leyendo valor analógico del MCP3008...\n");

    while (1) {
        // Leer valor analógico del MCP3008 (canal 1)
        int analogValue = LeerCanalSPI(1);
        printf("Valor analógico del MCP3008: %d\n", analogValue);

        // Esperar antes de la próxima lectura
        sleep(1);
    }

    gpioTerminate();
    return 0;
}
