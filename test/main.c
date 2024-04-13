#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>

#define TRIG_PIN 23
#define ECHO_PIN 24

int main()
{
    if (gpioInitialise() < 0)
    {
        printf("Error initializing pigpio library.\n");
        return 1;
    }

    gpioSetMode(TRIG_PIN, PI_OUTPUT);
    gpioSetMode(ECHO_PIN, PI_INPUT);

    while (1)
    {
        // Genera un pulso de 10 μs en el pin TRIG
        gpioWrite(TRIG_PIN, 1);
        gpioDelay(10);
        gpioWrite(TRIG_PIN, 0);

        // Espera hasta que el pin ECHO se ponga en alto
        while (gpioRead(ECHO_PIN) == 0)
        {
            // Espera activa
        }
        uint32_t start = gpioTick();

        // Espera hasta que el pin ECHO se ponga en bajo
        while (gpioRead(ECHO_PIN) == 1)
        {
            // Espera activa
        }
        uint32_t end = gpioTick();

        // Calcula la duración del pulso ultrasónico
        uint32_t pulse_duration = end - start;

        // Convierte la duración en distancia (cm)
        double distance_cm = pulse_duration / 58.0;

        printf("Distancia: %.2f cm\n", distance_cm);

        // Espera 1 segundo antes de la siguiente medición
        gpioDelay(1000000);
    }

    gpioTerminate();
    return 0;
}
