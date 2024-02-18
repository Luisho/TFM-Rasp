#include <stdio.h>
#include "maquina_estados.h"

// Guardar el estado Actual
enum Estado estadoActual = PARADA;

// Cambiar estado Actual
void cambiarEstado(enum Estado estadoNuevo){
    estadoActual = estadoNuevo;
}

// Función para manejar eventos
void manjarEstados(enum Evento evento){
    switch (estadoActual){
        case AVANZAR:
                
            break;
        case MARCHA_ATRAS:

            break;
        case GIRAR_DERECHA:

            break;
        case GIRAR_IZQUIERDA:

        break;
        case PARADA:

            break;
        default:
            break;
        }
}