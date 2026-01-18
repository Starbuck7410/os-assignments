TARGET = hw5
SOURCE = $(wildcard src/*.c)
INCLUDE = $(wildcard include/*.h)
CC = gcc
FLAGS = -Wall -Wextra
DEBUG_FLAGS = -Wall -Wextra -g -O0 -DDEBUG

# Default rule: build the program
$(TARGET): $(SOURCE) $(INCLUDE)
	@$(CC) $(FLAGS) -o $(TARGET) $(SOURCE)


test: $(TARGET)
	@./$(TARGET) 

fun: $(TARGET)
	@$(CC) $(FLAGS) -DFUNMODE -o $(TARGET) $(SOURCE)

test-fun: fun
	@./$(TARGET) fs.img ls

debug:
	@echo "Compiling with debug flags..."
	@$(CC) $(DEBUG_FLAGS) -o $(TARGET) $(SOURCE)
	@echo "Launching gdb..."
	@gdb ./$(TARGET) $(ARGS)

clean:
	@rm -f $(TARGET)


.PHONY: test debug fun test-fun
