#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>

#define TRIG_PIN 23
#define ECHO_PIN 24

// Prototipos de funciones
int initialize();
void cleanup();
void sendTriggerPulse();
uint32_t measurePulseDuration();
double calculateDistance(uint32_t pulse_duration);

int main() {
    if (initialize() < 0) {
        printf("Error initializing pigpio library.\n");
        return 1;
    }

    while (1) {
        sendTriggerPulse();
        uint32_t pulse_duration = measurePulseDuration();
        double distance_cm = calculateDistance(pulse_duration);
        printf("Distancia: %.2f cm\n", distance_cm);
        gpioDelay(1000000); // Espera 1 segundo antes de la siguiente medición
    }

    cleanup();
    return 0;
}

// Función para inicializar la biblioteca pigpio y configurar los pines
int initialize() {
    if (gpioInitialise() < 0) {
        return -1;
    }
    gpioSetMode(TRIG_PIN, PI_OUTPUT);
    gpioSetMode(ECHO_PIN, PI_INPUT);
    return 0;
}

// Función para limpiar y terminar la biblioteca pigpio
void cleanup() {
    gpioTerminate();
}

// Función para enviar un pulso de trigger al sensor
void sendTriggerPulse() {
    gpioWrite(TRIG_PIN, 1);
    gpioDelay(10);
    gpioWrite(TRIG_PIN, 0);
}

// Función para medir la duración del pulso ultrasónico
uint32_t measurePulseDuration() {
    while (gpioRead(ECHO_PIN) == 0);
    uint32_t start = gpioTick();
    while (gpioRead(ECHO_PIN) == 1);
    uint32_t end = gpioTick();
    return end - start;
}

// Función para calcular la distancia basada en la duración del pulso
double calculateDistance(uint32_t pulse_duration) {
    return pulse_duration / 58.0;
}
