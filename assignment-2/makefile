# You can run with:
# make run ARGS="test_cmd.txt 3 3 1"
TARGET = hw2
SOURCE = src/*
INCLUDE = include/*
CC = gcc
FLAGS = -Wall -Wextra -pthread
DEBUG_FLAGS = -Wall -Wextra -pthread -g -O0 -DDEBUG

# Default rule: build the program
$(TARGET): $(SOURCE) $(INCLUDE)
	@$(CC) $(FLAGS) -o $(TARGET) $(SOURCE)


run: clean $(TARGET)
	@./$(TARGET) $(ARGS)

fun: clean
	@$(CC) $(FLAGS) -DFUNMODE -o $(TARGET) $(SOURCE)
	@./$(TARGET) $(ARGS)

debug:
	@echo "Compiling with debug flags..."
	@$(CC) $(DEBUG_FLAGS) -o $(TARGET) $(SOURCE)
	@echo "Launching gdb..."
	@gdb ./$(TARGET) $(ARGS)

clean:
	@rm -f $(TARGET)

clean-all:
	@rm -f $(TARGET)
	@rm -f thread*.txt stats.txt dispatcher.txt count*.txt

.PHONY: run debug fun
