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
int keepalive = 60;

// MQTT Initialization
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

void iniciarMosquitto() {
    // Inicializar el cliente Mosquitto
    mosquitto_lib_init();
    // Crear una instancia del cliente Mosquitto
    mqtt_client = mosquitto_new(NULL, true, NULL);
    if (!mqtt_client) {
        fprintf(stderr, "Error al crear la instancia del cliente Mosquitto\n");
        return;
    }
    // Establecer callbacks
    mosquitto_connect_callback_set(mqtt_client, on_connect);
    mosquitto_publish_callback_set(mqtt_client, on_publish);
    // Conectar al broker MQTT
    if (mosquitto_connect(mqtt_client, MQTT_HOST, MQTT_PORT, keepalive) != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Error al conectar al broker MQTT\n");
        return;
    }
    // Iniciar el bucle de eventos MQTT en segundo plano
    mosquitto_loop_start(mqtt_client);
}

// Funciones
void movimientos(int value) {
    if (value >= 0 && value <= 1024) {
        char value_str[6]; 
        snprintf(value_str, sizeof(value_str), "%d", value);
        mosquitto_publish(mqtt_client, NULL, TOPIC_INST, strlen(value_str), value_str, QoS_2, false);
        printf(" Valor: %d\n", value);
    }
}

void marchaAtras() {
    int value = ATRAS_VALUE;
    char value_str[8];
    snprintf(value_str, sizeof(value_str), "%d", value);
    mosquitto_publish(mqtt_client, NULL, TOPIC_INST, strlen(value_str), value_str, QoS_2, false);
    printf(" Atras \n");
}

void enviarBoton() {
    int value = BOTON_PULSADO;
    char value_str[8];
    snprintf(value_str, sizeof(value_str), "%d", value);
    mosquitto_publish(mqtt_client, NULL, TOPIC_INST, strlen(value_str), value_str, QoS_2, false);
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
