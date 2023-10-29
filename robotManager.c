#include <stdio.h>
#include <pigpio.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>

#include "dictionary.h"
#include "robotManager.h"

int current_speed = -1;

void movimientos(int value){
    if (value >= 0) {
        if(value > LIMIT_IZ ){
            printf("girando a la izquierda");
            //hacer las llamadas para giro a la izquierda
        }else if(value < LIMIT_DE){
            printf("girando a la derecha");
            //hacer las llamadas para giro a la derecha
        }else{
            printf("Para alante");
            //hacer las llamadas para ir de frente
        }
        printf(" Valor: %d\n", value);
    }
}

void velocidad(int value){
    int speed;
    if(value < VEL_LIMIT_1)
        speed = 1;
    else if(value < VEL_LIMIT_2)
        speed = 2;
    else if(value < VEL_LIMIT_3)
        speed = 3;
    else
        speed = 4;
    
    if(speed != current_speed){
        printf("Velocidad establecida a: %d \n", speed);
        current_speed = speed;
    }
}