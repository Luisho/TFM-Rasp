enum Estado {GIRAR_IZQUIERDA, GIRAR_DERECHA, PARADA, AVANZAR, MARCHA_ATRAS};

enum Evento {giro_iz, giro_de, botonPulsado, retroceder};

void Giro_iz(int value);
void Giro_de(int value);
void BotonPulsado();
void Retroceder();
void generarEvento(const int instruccion);