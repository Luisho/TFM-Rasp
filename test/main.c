#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>

#define TRIG_PIN 12
#define ECHO_PIN 16
#define IR_SENSOR_PIN_DE 20
#define IR_SENSOR_PIN_IZ 21

// Prototipos de funciones
int initialize();
void cleanup();
void sendTriggerPulse();
uint32_t measurePulseDuration();
double calculateDistance(uint32_t pulse_duration);

int main() {
    if (initialize() < 0) {
        fprintf(stderr, "Error al inicializar pigpio\n");
        return 1;
    }

    gpioSetMode(IR_SENSOR_PIN_DE, PI_INPUT);
    gpioSetMode(IR_SENSOR_PIN_IZ, PI_INPUT);

    while (1) {
        sendTriggerPulse();
        uint32_t pulse_duration = measurePulseDuration();
        double distance_cm = calculateDistance(pulse_duration);
        printf("Distancia ultrasónica: %.2f cm\n", distance_cm);

        // Leer el valor del primer sensor infrarrojo
        int ir_value_1 = gpioRead(IR_SENSOR_PIN_DE);
        
        // Leer el valor del segundo sensor infrarrojo
        int ir_value_2 = gpioRead(IR_SENSOR_PIN_IZ);

        // Verificar el estado del primer sensor
        if (ir_value_1 == PI_LOW) {
            printf("Objeto detectado por el sensor infrarrojo derecho\n");
        } else {
            printf("Ningún objeto detectado por el sensor infrarrojo derecho\n");
        }

        // Verificar el estado del segundo sensor
        if (ir_value_2 == PI_LOW) {
            printf("Objeto detectado por el sensor infrarrojo izquierdo\n");
        } else {
            printf("Ningún objeto detectado por el sensor infrarrojo izquierdo\n");
        }

        // Esperar un corto período antes de tomar otra lectura
        gpioDelay(100000); // 100 ms
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

// Función para enviar un pulso de trigger al sensor ultrasónico
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

// Función para calcular la distancia basada en la duración del pulso ultrasónico
double calculateDistance(uint32_t pulse_duration) {
    return pulse_duration / 58.0; // Distancia en centímetros
}
