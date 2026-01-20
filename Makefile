CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LDFLAGS =

TARGET = bin/program
BUILD_DIR = build

SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Compilação concluída: $(TARGET)"

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) bin
	@echo "Limpeza concluída"

re: clean all

valgrind: ./bin/program
	valgrind --leak-check=full -s ./bin/program < ./testes/teste1.in

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean re run