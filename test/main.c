#include <stdio.h>
#include <pigpio.h>
#include <unistd.h> // Para la función sleep

#define HW691_ADDR 0x5A // Dirección I2C del sensor HW-691
#define INTERVALO_LECTURA_US 500000 // Intervalo de lectura en microsegundos (0.5 segundos)

int main() {
    if (gpioInitialise() < 0) {
        fprintf(stderr, "Error al inicializar pigpio\n");
        return 1;
    }

    int handle = i2cOpen(1, HW691_ADDR, 0); // Abre un bus I2C en la interfaz 1 y devuelve un identificador de manejo

    if (handle < 0) {
        fprintf(stderr, "Error al abrir el bus I2C del sensor HW-691\n");
        gpioTerminate();
        return 1;
    }

    printf("Leyendo temperaturas...\n");

    while (1) {
        // Leer temperatura ambiente en grados Celsius
        float ambientTempC = i2cReadWordData(handle, 0x06) * 0.02 - 273.15;

        // Leer temperatura del objeto en grados Celsius
        float objectTempC = i2cReadWordData(handle, 0x07) * 0.02 - 273.15;

        printf("Temperatura ambiente: %.2f °C\tTemperatura objeto: %.2f °C\n", ambientTempC, objectTempC);

        // Esperar antes de la próxima lectura
        usleep(INTERVALO_LECTURA_US);
    }

    // Nunca se llegará aquí, pero cerramos el bus I2C y terminamos pigpio para ser formales
    i2cClose(handle);
    gpioTerminate();

    return 0;
}
