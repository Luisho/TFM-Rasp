int Inicializar_SPI();
int LeerCanalSPI(int channel);
void Cerrar_SPI();
void initDevices();
int leerInfrarrojos();
void buttonPressed(int gpio, int level, uint32_t tick);