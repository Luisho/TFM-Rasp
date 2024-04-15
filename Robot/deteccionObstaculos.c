#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>
#include <signal.h>
#include "dictionary.h"

extern volatile sig_atomic_t thread_flag;

//Declaración funciones privadas
void sendTriggerPulse();
uint32_t measurePulseDuration();
double calculateDistance(uint32_t pulse_duration);

// Programa
void *deteccionObstaculosThread(void *arg) {
    // Inicialización de pines
    // initialize();
    gpioSetMode(IR_SENSOR_PIN_DE, PI_INPUT);
    gpioSetMode(IR_SENSOR_PIN_IZ, PI_INPUT);

    int ir_Sensor_De;
    int ir_Sensor_Iz;

    while (!thread_flag) {
        // Lógica de detección de obstáculos
        sendTriggerPulse();
        uint32_t pulse_duration = measurePulseDuration();
        double distance_cm = calculateDistance(pulse_duration);
        printf("Distancia ultrasónica: %.2f cm\n", distance_cm);

        // Leer el valor del primer sensor infrarrojo
        ir_Sensor_De = gpioRead(IR_SENSOR_PIN_DE);
        
        // Leer el valor del segundo sensor infrarrojo
        ir_Sensor_Iz = gpioRead(IR_SENSOR_PIN_IZ);

        // Verificar el estado del primer sensor
        if (ir_Sensor_De == PI_LOW) {
            printf("Objeto detectado por el sensor infrarrojo derecho\n");
        } else {
            printf("Ningún objeto detectado por el sensor infrarrojo derecho\n");
        }

        // Verificar el estado del segundo sensor
        if (ir_Sensor_Iz == PI_LOW) {
            printf("Objeto detectado por el sensor infrarrojo izquierdo\n");
        } else {
            printf("Ningún objeto detectado por el sensor infrarrojo izquierdo\n");
        }

        // Esperar un corto período antes de tomar otra lectura
        gpioDelay(100000); // 100 ms
    }

    pthread_exit(NULL);
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