#include <stdio.h>
#include <pigpio.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <mosquitto.h>
#include "dictionary.h"
#include "maquina_estados.h"
#include "controlarMotores.h"

struct mosquitto *mosq = NULL;
volatile sig_atomic_t  shouldExit = false;
enum Estado estadoActual = PARADA;
volatile sig_atomic_t thread_flag = 0;

void CtrlC_Interrupt(int signum) {
    shouldExit = true;
}

void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message) {
    // Guardar la instrucción en un entero
    if(strcmp(message->topic, TOPIC_INST) == 0){
        int instruccion;
        if (sscanf(message->payload, "%d", &instruccion) == 1) {
            //printf("Mensaje recibido en el topic: %s\n", message->topic);
            //printf("Contenido del mensaje: %d\n", instruccion);

            // llamada a control de motores
            generarEvento(instruccion);
        } else {
            printf("El mensaje no es un entero válido. %d\n",instruccion);
        }
    } else if (strcmp(message->topic, TOPIC_VEL) == 0){
        int velocidad;
        if (sscanf(message->payload, "%d", &velocidad) == 1) {
            printf("Mensaje recibido en el topic: %s\n", message->topic);
            printf("Contenido del mensaje: %d\n", velocidad);
            establecerVelocidad(velocidad);
        }
    }
}

int main() {
    /// Inicializar pigpio
    gpioInitialise();
    // Capturar señal ctrl+c
    signal(SIGINT, CtrlC_Interrupt);
    /// Inicializar pines motores
    initMotores();
    
    // Inicialización de Mosquitto
    mosquitto_lib_init();

    // Crear una instancia del cliente Mosquitto
    mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq) {
        fprintf(stderr, "Error al crear el cliente Mosquitto.\n");
        mosquitto_lib_cleanup();
        return 1;
    }

    // Establecer la función de mensaje recibido
    mosquitto_message_callback_set(mosq, on_message);

    // Conectar al broker Mosquitto
    int rc = mosquitto_connect(mosq, MQTT_HOST, MQTT_PORT, 60);
    if (rc != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Error al conectar con el broker Mosquitto, código: %d\n", rc);
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return 1;
    }
    mosquitto_subscribe(mosq, NULL, TOPIC_INST, QoS_2);
    mosquitto_subscribe(mosq, NULL, TOPIC_VEL, QoS_2);

    // Creación de un hilo para la detección de obstáculos
    pthread_t detection_thread;
    if (pthread_create(&detection_thread, NULL, deteccionObstaculosThread, NULL) != 0) {
        fprintf(stderr, "Error al crear el hilo de detección de obstáculos.\n");
        return 1;
    }
    
    // Bucle principal
    while (!shouldExit) {
        // Procesar eventos de Mosquitto
        int rc = mosquitto_loop(mosq, 0, 1);

        if (rc != MOSQ_ERR_SUCCESS) {
            fprintf(stderr, "Error en el bucle de Mosquitto, código: %d\n", rc);
            break;
        }
    }

    // Código de desconexión MQTT y limpieza
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    thread_flag = 1;
    return 0;
}
