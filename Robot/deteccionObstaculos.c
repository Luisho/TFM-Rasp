#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>
#include "dictionary.h"

extern volatile sig_atomic_t thread_flag = 0;

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