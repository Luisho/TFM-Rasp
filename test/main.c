#include <stdio.h>
#include <pigpio.h>

#define GY30_ADDR 0x5c // Dirección I2C del sensor GY-30

int main() {
    if (gpioInitialise() < 0) {
        printf("Error al inicializar pigpio\n");
        return 1;
    }

    int handle = i2cOpen(1, GY30_ADDR, 0); // Abre un bus I2C en la interfaz 1 y devuelve un identificador de manejo

    if (handle < 0) {
        printf("Error al abrir el bus I2C\n");
        return 1;
    }

    while(1) { // Realizar 10 lecturas de luminosidad
        // Escribir un comando para iniciar la medición en el sensor de luminosidad
        if (i2cWriteByte(handle, 0x10) != 0) {
            printf("Error al enviar el comando de inicio de medición\n");
            return 1;
        }

        // Esperar un breve tiempo para que el sensor realice la medición
        time_sleep(0.5);

        // Leer datos del sensor
        uint16_t data;
        data = i2cReadWordData(handle, 0x00);
        if (data < 0) {
            printf("Error al leer datos del sensor\n");
            return 1;
        }
        

        // Convertir los datos a lux
        double lux = data / 1.2;

        printf("Luminosidad: %.2f lux\n", lux);
    }

    // Cerrar el bus I2C
    i2cClose(handle);

    gpioTerminate(); // Terminar pigpio

    return 0;
}
