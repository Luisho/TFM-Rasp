#include <stdio.h>
#include "maquina_estados.h"
#include "controlarMotores.h"
#include "dictionary.h"

// Guardar el estado Actual
extern enum Estado estadoActual;

//Generador de Eventos
void generarEvento(const int instruccion) {
    // Lógica de control de motores según la instrucción recibida
    //printf("%d", instruccion);
    if(instruccion <= GIRO_IZ_LIMIT_VALUE)
        Giro_iz(instruccion);
    else if(instruccion >= GIRO_DE_LIMIT_VALUE && instruccion <= MAX_POT_VALUE)
        Giro_de(instruccion);
    else if(instruccion == ATRAS_VALUE)
        Retroceder(); 
    else if(instruccion == BOTON_PULSADO)
        BotonPulsado();
    else if(instruccion > GIRO_IZ_LIMIT_VALUE && instruccion < GIRO_DE_LIMIT_VALUE && (estadoActual == GIRAR_IZQUIERDA || estadoActual == GIRAR_DERECHA))
        BotonPulsado();
}

// Funciones para manejar los eventos
void Giro_iz(int value){
    switch (estadoActual) {
        case PARADA:
            estadoActual = GIRAR_IZQUIERDA;
            girarIzquierda(value);
            break;
        case GIRAR_DERECHA:
            estadoActual = GIRAR_IZQUIERDA;
            girarIzquierda(value);
            break;
        case GIRAR_IZQUIERDA:
            girarIzquierda(value); 
            break;
        case AVANZAR:
            break;
        case MARCHA_ATRAS:
            break;
        default:
            break;
    }
}

void Giro_de(int value){
    switch (estadoActual) {
        case PARADA:
            estadoActual = GIRAR_DERECHA;
            girarDerecha(value);
            break;
        case GIRAR_IZQUIERDA:
            estadoActual = GIRAR_DERECHA;
            girarDerecha(value);
            break;
        case GIRAR_DERECHA:
            girarDerecha(value); 
            break;
        case AVANZAR:
            break;
        case MARCHA_ATRAS:
            break;
        default:
            break;
    }
}

void BotonPulsado(){
    switch (estadoActual) {
        case PARADA:
            estadoActual = AVANZAR;
            moverMotoresAlante();
            break;
        case GIRAR_IZQUIERDA:
            estadoActual = PARADA;
            pararMotores();
            break;
        case GIRAR_DERECHA:
            estadoActual = PARADA;
            pararMotores(); 
            break;
        case AVANZAR:
            estadoActual = PARADA;
            pararMotores(); 
            break;
        case MARCHA_ATRAS:
            estadoActual = PARADA;
            pararMotores();
            break;
        default:
            estadoActual = PARADA;
            pararMotores();
            break;
    }
}

void Retroceder(){
    switch (estadoActual) {
        case PARADA:
            estadoActual = MARCHA_ATRAS;
            moverMotoresAtras();
            break;
        case GIRAR_IZQUIERDA:
            break;
        case GIRAR_DERECHA:
            break;
        case AVANZAR:
            break;
        case MARCHA_ATRAS:
            break;
        default:
            break;
    }
}