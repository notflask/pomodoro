CC = gcc
CFLAGS = -Wall -g
TARGET = pomodoro
INSTALL_DIR = /usr/local/bin

build: $(TARGET)

$(TARGET): pomodoro.c pomodoro.h
	$(CC) $(CFLAGS) -o $(TARGET) pomodoro.c

run: build
	./$(TARGET)

install: build
	install -Dm755 $(TARGET) $(INSTALL_DIR)/$(TARGET)

uninstall:
	rm -f $(INSTALL_DIR)/$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: build run install uninstall clean

