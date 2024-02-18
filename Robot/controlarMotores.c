#include "controlarMotores.h"

int velocidadActual = 1;

void pararMotores(){
    // Código para parar motores
}

void moverMotoresAlante(){
    // Código Avanzar con una velocidad establecida "velocidadActual"
}

void moverMotoresAtras(){
    // Codigo mover los motores hacia atrás
}

void girarIzquierda(){
    // Codigo para girar a la izquirda
}

void girarDerecha(){
    // Código parar girar a la derecha
}

void establecerVelocidad(int value){
    if(velocidadActual != value){
        // Se actualiza el valor de velocidad
        velocidadActual = value; // El ponerlo así puede que venga bien en un futuro
    }
}