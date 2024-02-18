enum Estado {GIRAR_IZQUIERDA, GIRAR_DERECHA, PARADA, AVANZAR, MARCHA_ATRAS};

enum Evento {giro_iz, giro_de, Parar, avanzar, retroceder};

void manjarEstados(enum Evento evento);