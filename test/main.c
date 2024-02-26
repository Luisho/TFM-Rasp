#include <stdio.h>
#include <mosquitto.h>
#include <string.h>
#include <unistd.h>

// Global variables
struct mosquitto *mqtt_client;
int keepalive = 60;

// MQTT Configuration
#define MQTT_HOST "192.168.3.57"   // IP del broker MQTT
#define MQTT_PORT 1883             // Puerto del broker MQTT
#define QoS_2 2                    // Calidad de Servicio 2

void publicarMensaje(const char *topic, const char *mensaje, const int QoS);

// MQTT Callbacks
void on_connect(struct mosquitto *mosq, void *userdata, int result) {
    printf("1");
    if (result == MOSQ_ERR_SUCCESS) {
        printf("Conexión MQTT exitosa\n");
    } else {
        fprintf(stderr, "Error al conectar al broker MQTT: %s\n", mosquitto_connack_string(result));
    }
}

void on_publish(struct mosquitto *mosq, void *userdata, int mid) {
    printf("Mensaje MQTT publicado con éxito\n");
}

// MQTT Initialization
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
    // Conectar al broker MQTT (asegúrate de que el broker esté en ejecución)
    if (mosquitto_connect(mqtt_client, MQTT_HOST, MQTT_PORT, keepalive) != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Error al conectar al broker MQTT\n");
        return;
    }
    // Iniciar el bucle de eventos MQTT en segundo plano
    mosquitto_loop_start(mqtt_client);
}

// MQTT Cleanup
void finalizarMosquitto() {
    // Desconectar y destruir el cliente Mosquitto
    mosquitto_disconnect(mqtt_client);
    mosquitto_destroy(mqtt_client);

    // Finalizar la biblioteca Mosquitto
    mosquitto_lib_cleanup();
}

// Publish Test Messages
void publicarMensajesPrueba() {
    // Publica mensajes de prueba en el topic "Robot/instrucciones"
    publicarMensaje("Robot/instrucciones", "Giro izquierda", QoS_2);
    publicarMensaje("Robot/instrucciones", "Giro derecha", QoS_2);
    publicarMensaje("Robot/instrucciones", "Alante", QoS_2);
}

// Publish Message
void publicarMensaje(const char *topic, const char *mensaje, const int QoS) {
    // Publicar un mensaje en el topic especificado
    mosquitto_publish(mqtt_client, NULL, topic, strlen(mensaje), mensaje, QoS, false);
    printf("7");
}

int main() {
    iniciarMosquitto();
    

    // Publicar mensajes de prueba
    publicarMensajesPrueba();

    // Esperar unos segundos para permitir la entrega de mensajes
    sleep(5);

    finalizarMosquitto();

    return 0;
}
