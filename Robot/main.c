#include <stdio.h>
#include <pigpio.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <mosquitto.h>

// Definir pines de control de motores (ajusta según tus conexiones)
#define MOTOR1_PIN 17
#define MOTOR2_PIN 18

struct mosquitto *mosq = NULL;


void controlarMotores(const char *instruccion) {
    // Lógica de control de motores según la instrucción recibida
    if (strcmp(instruccion, "avanzar") == 0) {
        // Lógica para avanzar
        printf("Avanzando\n");
        // Llama a funciones o realiza operaciones específicas para avanzar
    } else if (strcmp(instruccion, "girar_izquierda") == 0) {
        // Lógica para girar a la izquierda
        printf("Girando a la izquierda\n");
        // Llama a funciones o realiza operaciones específicas para girar a la izquierda
    } else if (strcmp(instruccion, "girar_derecha") == 0) {
        // Lógica para girar a la derecha
        printf("Girando a la derecha\n");
        // Llama a funciones o realiza operaciones específicas para girar a la derecha
    } else {
        // Instrucción desconocida
        printf("Instrucción desconocida: %s\n", instruccion);
    }
}

void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message) {
    // Convertir el contenido del mensaje a una cadena de caracteres
    char instruccion[message->payloadlen + 1];
    memcpy(instruccion, message->payload, message->payloadlen);
    instruccion[message->payloadlen] = '\0';

    printf("Mensaje recibido en el topic: %s\n", message->topic);
    printf("Contenido del mensaje: %s\n", instruccion);

    // Lógica de control de motores
    controlarMotores(instruccion);
}

int main() {
    // (Código de conexión MQTT y suscripción, similar al ejemplo anterior)

    // Bucle principal
    while (1) {
        // Procesar eventos de Mosquitto
        int rc = mosquitto_loop(mosq, 0, 1);

        // Realizar otras operaciones según sea necesario
        // ...

        if (rc != MOSQ_ERR_SUCCESS) {
            fprintf(stderr, "Error en el bucle de Mosquitto, código: %d\n", rc);
            break;
        }
    }

    // (Código de desconexión MQTT y limpieza, similar al ejemplo anterior)
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}
