#define SPI_CHANNEL 0
#define SPI_SPEED   1000000  // Velocidad en Hz
#define SPI_FLAGS   0

// PINOUT
#define LED_RED 23
#define LED_YELLOW 24
#define BUTTON_GPIO 22

// ADC CHANNELS
#define POT_DESLIZANTE 3
#define POT_ROTATORIO 2
#define POT_CERMET 1
#define LUMIN 0

// MOVIMIENTOS LIMITES
#define LIMIT_IZ 600
#define LIMIT_DE 400

// VELOCIDAD LIMITES
// la velocidad va de 512: mínimo -> 1024: máximo
#define VEL_STOP 520
#define VEL_LIMIT_1 640
#define VEL_LIMIT_2 768
#define VEL_LIMIT_3 896

// MQTT
#define MQTT_HOST "192.168.3.57"            // IP del broker MQTT
#define MQTT_PORT 1883                      // Puerto del broker MQTT
#define TOPIC_VEL "Robot/Info/vel"          // Topic Velocidad
#define TOPIC_INST "Robot/instrucciones"    // Topic Instrucciones
#define QoS_0 0                             // Calidad de Servicio 0
#define QoS_1 1                             // Calidad de Servicio 1
#define QoS_2 2                             // Calidad de Servicio 2