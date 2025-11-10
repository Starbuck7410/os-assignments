# Name of the executable
TARGET = hw1shell
SOURCE = src/*
CC = gcc
FLAGS = -Wall -Wextra
DEBUG_FLAGS = -Wall -Wextra -g -O0 -DDEBUG

# Default rule: build the program
$(TARGET): $(SOURCE)
	@$(CC) $(FLAGS) -o $(TARGET) $(SOURCE)

# Run rule
run: $(TARGET)
	@./$(TARGET)

# Debug rule: build with debug flags and run gdb
debug:
	@echo "Compiling with debug flags..."
	@$(CC) $(DEBUG_FLAGS) -o $(TARGET) $(SOURCE)
	@echo "Launching gdb..."
	@gdb ./$(TARGET)

.PHONY: run debug
