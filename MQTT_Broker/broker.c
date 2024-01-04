#include <stdio.h>
#include <mosquitto.h>

// Constantes
#define MQTT_PORT 1883
#define IP_HOST "192.168.3.57"

// Estructura para el manejo del broker MQTT
struct mosquitto *mosq = NULL;

// Función llamada al conectarse al broker
void on_connect(struct mosquitto *mosq, void *userdata, int result) {
    if (result == 0) {
        // La conexión fue exitosa
        printf("Broker connection successful\n");

        // Se suscribe al topic 'example_topic' con calidad de servicio (QoS) 0
        mosquitto_subscribe(mosq, NULL, "example_topic", 0);
    } else {
        // La conexión falló
        fprintf(stderr, "Failed to connect to broker\n");
    }
}

// Función llamada al recibir un mensaje
void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message) {
    if (message->payloadlen) {
        // Se imprime el mensaje recibido
        printf("Received message on topic %s: %s\n", message->topic, (char *)message->payload);
    } else {
        // Mensaje vacío
        printf("Received empty message on topic %s\n", message->topic);
    }
}

int main() {
    // Inicialización de la biblioteca Mosquitto
    mosquitto_lib_init();

    // Creación de la estructura para el broker
    mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq) {
        // Manejo de error en caso de falta de memoria
        fprintf(stderr, "Error: Out of memory\n");
        return 1;
    }

    // Configuración de las funciones de callback
    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);

    // Conexión al broker MQTT
    if (mosquitto_connect(mosq, IP_HOST, MQTT_PORT, 60) != MOSQ_ERR_SUCCESS) {
        // Manejo de error en caso de no poder conectarse
        fprintf(stderr, "Unable to connect to the broker\n");
        return 1;
    }

    // Inicio del bucle de eventos en un hilo separado
    mosquitto_loop_start(mosq);

    // Desconexión y limpieza al finalizar el programa
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}
