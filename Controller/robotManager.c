// Librerias
#include <stdio.h>
#include <pigpio.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <mosquitto.h>

// Includes
#include "dictionary.h"
#include "robotManager.h"

// Global variables
int current_speed = -1;

// Estructuras
struct mosquitto *mqtt_client;

// Funciones
void on_connect(struct mosquitto *mosq, void *userdata, int result) {
    if (result == MOSQ_ERR_SUCCESS) {
        printf("Conexión MQTT exitosa\n");
    } else {
        fprintf(stderr, "Error al conectar al broker MQTT\n");
    }
}

void on_publish(struct mosquitto *mosq, void *userdata, int mid) {
    printf("Mensaje MQTT publicado con éxito\n");
}

void movimientos(int value) {
    if (value >= 0) {
        /*if (value > LIMIT_IZ) {
            printf("Girando a la izquierda\n");
            // Publica el giro en el topic "Robot/instrucciones"
            mosquitto_publish(mqtt_client, NULL, "Robot/instrucciones", sizeof("Giro izquierda"), "Giro izquierda", QoS_2, false);
        } else if (value < LIMIT_DE) {
            printf("Girando a la derecha\n");
            // Llama a funciones para girar a la derecha

            // Publica el giro en el topic "Robot/instrucciones"
            mosquitto_publish(mqtt_client, NULL, "Robot/instrucciones", sizeof("Giro derecha"), "Giro derecha", QoS_2, false);
        } else {
            printf("Para adelante\n");
            // Llama a funciones para avanzar

            // Publica el movimiento hacia adelante en el topic "Robot/instrucciones"
            mosquitto_publish(mqtt_client, NULL, "Robot/instrucciones", sizeof("Adelante"), "Adelante", QoS_2, false);
        }*/
        mosquitto_publish(mqtt_client, NULL, TOPIC_INST, sizeof(value), value, QoS_2, false);
        printf(" Valor: %d\n", value);
    }
}

void velocidad(int value) {
    //printf("potenciometro velocidad = %d\n", value);
    int speed;
    if (value < VEL_STOP)
        speed = 0;
    else if (value < VEL_LIMIT_1)
        speed = 1;
    else if (value < VEL_LIMIT_2)
        speed = 2;
    else if (value < VEL_LIMIT_3)
        speed = 3;
    else
        speed = 4;

    if (speed != current_speed) {
        printf("Velocidad establecida a: %d \n", speed);
        current_speed = speed;

        // Publica la velocidad en el topic "Robot/Info/vel"
        char speed_str[2];
        snprintf(speed_str, sizeof(speed_str), "%d", speed);
        mosquitto_publish(mqtt_client, NULL, TOPIC_VEL, sizeof(speed_str), speed_str, QoS_1, false);
    }
}
