// Librerias
#include <stdio.h>
#include <pigpio.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <mosquitto.h>
#include <string.h>

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
    //int value = (int)(((float)(valueToAjust-512)/512 )*1024);
    if (value >= 0) {
        mosquitto_publish(mqtt_client, NULL, TOPIC_INST, sizeof(value), &value, QoS_2, false);
        printf(" Valor: %d\n", value);
    }
}

void marchaAtras(){
    int value = ATRAS_VALUE;
    mosquitto_publish(mqtt_client, NULL, TOPIC_INST, sizeof(value), &value, QoS_2, false);
    printf(" Atras \n");
}

void enviarBoton(){
    int value = BOTON_PULSADO;
    mosquitto_publish(mqtt_client, NULL, TOPIC_INST, sizeof(value), &value, QoS_2, false);
    printf(" Parada \n");
}

void velocidad(int value) {
    //printf("potenciometro velocidad = %d\n", value);
    int speed;
    if (value == VEL_STOP)
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
