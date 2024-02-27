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