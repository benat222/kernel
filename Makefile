CC = gcc
CFLAGS = -Wall -pthread
TARGET = main
SOURCES = main.c clock.c timer.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

.PHONY: clean