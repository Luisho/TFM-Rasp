# Nombre del ejecutable
TARGET = monitor.elf

# Compilador y opciones de compilación
CC = gcc
CFLAGS = -Wall -I.
LDFLAGS = -lpigpio -lrt -lmosquitto

# Fuentes y objetos
SRCS = main.c SensorsManager.c CommunicationModule.c #Añadir los .c
OBJS = $(SRCS:.c=.o)

# Reglas de construcción
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Regla para eliminar archivos objeto y el ejecutable
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean

