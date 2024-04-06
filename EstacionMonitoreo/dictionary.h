// MQTT
#define MQTT_HOST "192.168.3.57"                        // IP del broker MQTT
#define MQTT_PORT 1883                                  // Puerto del broker MQTT
#define TOPIC_LUM "dashboard/luminity"                  // Topic luminosidad
#define TOPIC_AMB_TEMP "dashboard/AmbientTemperature"   // Topic luminosidad
#define TOPIC_OBJ_TEMP "dashboard/ObjectTemperature"    // Topic luminosidad
#define TOPIC_GAS "dashboard/Gases"                     // Topic luminosidad
#define QoS_0 0                                         // Calidad de Servicio 0
#define QoS_1 1                                         // Calidad de Servicio 1
#define QoS_2 2                                         // Calidad de Servicio 2

// CONFIGURACION SENSORES
#define GY30_ADDRESS 0x5C
#define HW691_ADDRESS 0x5a
#define AMBIENT_TEMP_CHANNEL 0x06
#define OBJECT_TEMP_CHANNEL 0x07
#define GASES_CHANNEL 1

// CANAL SPI
#define SPI_CHANNEL 0
#define SPI_SPEED   1000000  // Velocidad en Hz
#define SPI_FLAGS   0