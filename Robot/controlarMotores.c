#include <stdio.h>
#include <pigpio.h>
#include "controlarMotores.h"
#include "dictionary.h"

// Puente H utilizado es el  L298N
// Los motores utilizados son DC

int velocidadActual = STOP_VALUE;
int valorPWM = STOP_VALUE;

// Funciones
void pararMotores(){
    // Código para parar motores
    gpioPWM(MOTOR_D_PWM, STOP_VALUE);
    gpioWrite(MOTOR_D_IN1, LOW);
    gpioWrite(MOTOR_D_IN2, LOW);
    
    gpioPWM(MOTOR_I_PWM, STOP_VALUE);
    gpioWrite(MOTOR_I_IN1, LOW);
    gpioWrite(MOTOR_I_IN2, LOW);

    printf("Parar\n");
}

void moverMotoresAlante(){
    // Código Avanzar con una velocidad establecida "velocidadActual"
    // deberá establecer la velocidad de los motores según el valor de la variable global
    gpioWrite(MOTOR_D_IN1, HIGH);
    gpioWrite(MOTOR_D_IN2, LOW);
    gpioPWM(MOTOR_D_PWM, valorPWM);

    gpioWrite(MOTOR_I_IN1, HIGH);
    gpioWrite(MOTOR_I_IN2, LOW);
    gpioPWM(MOTOR_I_PWM, valorPWM);
    printf("Avanzando\n");
}

void moverMotoresAtras(){
    // Codigo mover los motores hacia atrás
    // Deberá ser a una velocidad fija no cambiará
    gpioWrite(MOTOR_D_IN1, LOW);
    gpioWrite(MOTOR_D_IN2, HIGH);
    gpioPWM(MOTOR_D_PWM, ATRAS_SPEED);

    gpioWrite(MOTOR_I_IN1, LOW);
    gpioWrite(MOTOR_I_IN2, HIGH);
    gpioPWM(MOTOR_I_PWM, ATRAS_SPEED);

    printf("Moviéndo hacia atrás\n");
}

void girarIzquierda(int value){
    // Codigo para girar a la izquirda sobre su propio eje
    // Formatear el valor recibido a un valor pwm
    int pwm_valor = (value - IZQUIERDA_MIN_VALUE) * (MAX_PWM_VALUE) / (IZQUIERDA_MAX_VALUE - IZQUIERDA_MIN_VALUE);
    gpioWrite(MOTOR_D_IN1, HIGH);
    gpioWrite(MOTOR_D_IN2, LOW);
    gpioPWM(MOTOR_D_PWM, pwm_valor);

    gpioWrite(MOTOR_I_IN1, LOW);
    gpioWrite(MOTOR_I_IN2, HIGH);
    gpioPWM(MOTOR_I_PWM, pwm_valor);
    printf("Girando a la izquierda con valor: %d\n", value);
}

void girarDerecha(int value){
    // Código parar girar a la derecha sobre su propio eje
    int pwm_valor = (value - DERECHA_MIN_VALUE) * (MAX_PWM_VALUE) / (DERECHA_MAX_VALUE - DERECHA_MIN_VALUE);
    gpioWrite(MOTOR_D_IN1, LOW);
    gpioWrite(MOTOR_D_IN2, HIGH);
    gpioPWM(MOTOR_D_PWM, pwm_valor);

    gpioWrite(MOTOR_I_IN1, HIGH);
    gpioWrite(MOTOR_I_IN2, LOW);
    gpioPWM(MOTOR_I_PWM, pwm_valor);
    printf("Girando a la derecha con valor: %d\n", value);
}

void establecerVelocidad(int value){
    if(velocidadActual != value){
        // Se actualiza el valor de velocidad
        velocidadActual = value; // El ponerlo así puede que venga bien en un futuro
        
        // Modelizado del valor PWM respecto de la velocidad de 4 etapas establecida
        valorPWM = ((value * 25) * 255) / 100;
    }
}

void initMotores() {
    // Pines motor derecho
    gpioSetMode(MOTOR_D_IN1, PI_OUTPUT);
    gpioSetMode(MOTOR_D_IN2, PI_OUTPUT);
    gpioSetMode(MOTOR_D_PWM, PI_OUTPUT);
    
    // Pines motor izquierdo
    gpioSetMode(MOTOR_I_IN1, PI_OUTPUT);
    gpioSetMode(MOTOR_I_IN2, PI_OUTPUT);
    gpioSetMode(MOTOR_I_PWM, PI_OUTPUT);
}