CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = -lmosquitto

SRCS = broker.c
OBJS = $(SRCS:.c=.o)

TARGET = mqttBroker.elf

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
