#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>

#define IR_SENSOR1_PIN 17
#define IR_SENSOR2_PIN 18
#define ULTRASONIC_TRIGGER_PIN 23
#define ULTRASONIC_ECHO_PIN 24

void setup() {
    if (gpioInitialise() < 0) {
        fprintf(stderr, "Error al inicializar pigpio\n");
        return;
    }
    gpioSetMode(IR_SENSOR1_PIN, PI_INPUT);
    gpioSetMode(IR_SENSOR2_PIN, PI_INPUT);
    gpioSetMode(ULTRASONIC_TRIGGER_PIN, PI_OUTPUT);
    gpioSetMode(ULTRASONIC_ECHO_PIN, PI_INPUT);
}

void cleanup() {
    gpioTerminate();
}

int read_ir_sensor(int pin) {
    return gpioRead(pin);
}

float read_ultrasonic_sensor() {
    gpioTrigger(ULTRASONIC_TRIGGER_PIN, 10, 1); // Generar pulso de 10 µs en el pin de trigger
    gpioTime_t start, end;
    gpioTime(0, &start);
    gpioTime(0, &end);
    while (!gpioRead(ULTRASONIC_ECHO_PIN)) {
        gpioTime(0, &start);
    }
    while (gpioRead(ULTRASONIC_ECHO_PIN)) {
        gpioTime(0, &end);
    }
    uint32_t diff = gpioDiff(&start, &end); // Diferencia de tiempo en microsegundos
    return (float)diff / 1000000.0 * 34300.0 / 2.0; // Calcular distancia en centímetros
}

int main() {
    if (gpioInitialise() < 0) {
        fprintf(stderr, "Error al inicializar pigpio\n");
        return 1;
    }

    setup();

    while (1) {
        int ir1 = read_ir_sensor(IR_SENSOR1_PIN);
        int ir2 = read_ir_sensor(IR_SENSOR2_PIN);
        float distance = read_ultrasonic_sensor();
        printf("IR Sensor 1: %d\n", ir1);
        printf("IR Sensor 2: %d\n", ir2);
        printf("Ultrasonic Distance: %.2f cm\n", distance);
        sleep(1);
    }

    cleanup();

    return 0;
}
