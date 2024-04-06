// Librerias
#include <stdio.h>
#include <pigpio.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <mosquitto.h>
#include <string.h>

#include "dictionary.h"
#include "CommunicationModule.h"

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

void publicLum(int lum){
    char payload[10];
    sprintf(payload, "%d", lum);
    
    // Publicar el valor de luminosidad en el topic adecuado
    int ret = mosquitto_publish(mqtt_client, NULL, TOPIC_LUM, strlen(payload), payload, 0, false);
    if (ret != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Error al publicar el valor de luminosidad\n");
        return;
    }
}

void publicGas(int gas){
    int value = (gas / 1024.0) * 100.0;
    char payload[10];
    sprintf(payload, "%d", value);
    
    // Publicar el valor de luminosidad en el topic adecuado
    int ret = mosquitto_publish(mqtt_client, NULL, TOPIC_GAS, strlen(payload), payload, 0, false);
    if (ret != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Error al publicar el valor de Gases\n");
        return;
    }
}

void publicAmbTemp(float value){
    if(value > -270){
        char payload[20];
        sprintf(payload, "%.2f", value); // "%.2f" para dos decimales
        
        // Publicar el valor de luminosidad en el topic adecuado
        int ret = mosquitto_publish(mqtt_client, NULL, TOPIC_AMB_TEMP, strlen(payload), payload, 0, false);
        if (ret != MOSQ_ERR_SUCCESS) {
            fprintf(stderr, "Error al publicar el valor de Temperatura Ambiente\n");
            return;
        }
    }
}

void publicObjTemp(float value){
    if(value > -270){
        char payload[20];
        sprintf(payload, "%.2f", value); // "%.2f" para dos decimales
        
        // Publicar el valor de luminosidad en el topic adecuado
        int ret = mosquitto_publish(mqtt_client, NULL, TOPIC_OBJ_TEMP, strlen(payload), payload, 0, false);
        if (ret != MOSQ_ERR_SUCCESS) {
            fprintf(stderr, "Error al publicar el valor de Temperatura del objeto\n");
            return;
        }
    }
}