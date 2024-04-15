#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include "dictionary.h"
#include "maquina_estados.h"


// Variables externas
extern volatile sig_atomic_t thread_flag;
extern enum Estado estadoActual;
extern int Obstaculo_detectado;
extern pthread_mutex_t generarEvento_mutex;

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

        // Comprobar detección obstáculo
        if(distance_cm < MAX_DISTANCE || ir_Sensor_De == PI_LOW || ir_Sensor_Iz == PI_LOW){
            printf("Obstaculo detectado  ");
            Obstaculo_detectado = 1;
            if(estadoActual == AVANZAR){
                pthread_mutex_lock(&generarEvento_mutex);
                generarEvento(BOTON_PULSADO);
                pthread_mutex_unlock(&generarEvento_mutex);
            }
        }else{
            printf("No se ha detectado obstaculos  ");
            Obstaculo_detectado = 0;
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