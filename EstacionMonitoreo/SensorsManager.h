int initDevices(int *handleLum, int *handleTemp);
void getluminityValues(int handleLum, int *lum);
void getAmbientTemperature(int handleTemp, float *ambientTemp);
void getObjectTemperature(int handleTemp, float *objectTemp);
int Inicializar_SPI(int *spi_handle);
int LeerCanalSPI(const int channel, int *spi_handle);