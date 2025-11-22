TARGET = hw1shell
SOURCE = src/*
INCLUDE = include/*
CC = gcc
FLAGS = -Wall -Wextra
DEBUG_FLAGS = -Wall -Wextra -g -O0 -DDEBUG

# Default rule: build the program
$(TARGET): $(SOURCE) $(INCLUDE)
	@$(CC) $(FLAGS) -o $(TARGET) $(SOURCE)


run: clean $(TARGET)
	@./$(TARGET)

fun: clean
	@$(CC) $(FLAGS) -DFUNMODE -o $(TARGET) $(SOURCE)
	@./$(TARGET)

debug:
	@echo "Compiling with debug flags..."
	@$(CC) $(DEBUG_FLAGS) -o $(TARGET) $(SOURCE)
	@echo "Launching gdb..."
	@gdb ./$(TARGET)

clean:
	@rm -f $(TARGET)	

.PHONY: run debug fun
