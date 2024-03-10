// MQTT
#define MQTT_HOST "192.168.3.57"            // IP del broker MQTT
#define MQTT_PORT 1883                      // Puerto del broker MQTT
#define TOPIC_VEL "Robot/Info/vel"          // Topic Velocidad
#define TOPIC_INST "Robot/instrucciones"    // Topic Instrucciones
#define QoS_0 0                             // Calidad de Servicio 0
#define QoS_1 1                             // Calidad de Servicio 1
#define QoS_2 2                             // Calidad de Servicio 2
#define BOTON_PULSADO 3333                  // valor para indicar que se ha pulsado el botón
#define ATRAS_VALUE 2222                    // valor para ir marcha atras

// Valores Potenciometro volante
// De esta forma se deja un punto muerto entre los valores ya que la precisión del portenciómetro no es muy exacta
#define GIRO_IZ_LIMIT_VALUE 481             // Valor que limita el giro a la izquierda
#define GIRO_DE_LIMIT_VALUE 542             // Valor que limita el giro a la derecha
#define MAX_POT_VALUE 1024                  // Valor máximo del potenciómetro

// Pines puente H L298N
// Se defines dos pines por motor debido a que los IN sirven para la dirección y el PWM para la potencia
// Motor Derecho
#define MOTOR_D_IN1 23  // GPIO 23 - Control de dirección motor Derecho
#define MOTOR_D_IN2 24  // GPIO 24 - Control de dirección motor Derecho
#define MOTOR_D_PWM 18  // GPIO 18 - PWM para control de velocidad motor Derecho

// Motor Izquierdo
#define MOTOR_I_IN1 5  // GPIO 5 - Control de dirección motor Izquierdo
#define MOTOR_I_IN2 6  // GPIO 6 - Control de dirección motor Izquierdo
#define MOTOR_I_PWM 13 // GPIO 13 - PWM para control de velocidad motor Izquierdo

// Valores definidos
#define MAX_PWM_VALUE 255
#define MIN_PWM_VALUE 0
#define STOP_VALUE 0
#define ATRAS_VALUE 25
#define IZQUIERDA_MIN_VALUE 481
#define IZQUIERDA_MAX_VALUE 0
#define DERECHA_MIN_VALUE 542
#define DERECHA_MAX_VALUE 1024