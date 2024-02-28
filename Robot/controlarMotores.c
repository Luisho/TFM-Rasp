#include <stdio.h>
#include "controlarMotores.h"

int velocidadActual = 1;

void pararMotores(){
    // Código para parar motores
    printf("Parar\n");
}

void moverMotoresAlante(){
    // Código Avanzar con una velocidad establecida "velocidadActual"
    printf("Avanzando\n");
}

void moverMotoresAtras(){
    // Codigo mover los motores hacia atrás
    printf("Moviéndome hacia atrás\n");
}

void girarIzquierda(int value){
    // Codigo para girar a la izquirda
    printf("Girando a la izquierda con valor: %d\n", value);
}

void girarDerecha(int value){
    // Código parar girar a la derecha
    printf("Girando a la derecha con valor: %d\n", value);
}

void establecerVelocidad(int value){
    if(velocidadActual != value){
        // Se actualiza el valor de velocidad
        velocidadActual = value; // El ponerlo así puede que venga bien en un futuro
    }
}