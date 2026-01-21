TARGET = hw5
SOURCE = $(wildcard src/*.c)
INCLUDE = $(wildcard include/*.h)
CC = gcc
FLAGS = -Wall -Wextra
DEBUG_FLAGS = -Wall -Wextra -g -O0 -DDEBUG
IMAGE_FILE=fs.img
TEST_DIR=test
CP_TARGET=ln

# Default rule: build the program
$(TARGET): $(SOURCE) $(INCLUDE)
	@$(CC) $(FLAGS) -o $(TARGET) $(SOURCE)


test: $(TARGET)
	@mkdir -p $(TEST_DIR)
	@echo "--- Testing ls ---"
	@./$(TARGET) $(IMAGE_FILE) ls
	@echo "--- Testing cp ---"
	@./$(TARGET) $(IMAGE_FILE) cp $(CP_TARGET) $(TEST_DIR)/$(CP_TARGET)

fun: $(TARGET)
	@$(CC) $(FLAGS) -DFUNMODE -o $(TARGET) $(SOURCE)

test-fun: fun
	@./$(TARGET) $(IMAGE_FILE) ls

debug:
	@echo "Compiling with debug flags..."
	@$(CC) $(DEBUG_FLAGS) -o $(TARGET) $(SOURCE)
	@echo "Launching gdb..."
	@gdb ./$(TARGET) $(ARGS)

clean:
	@rm -rf $(TARGET) $(TEST_DIR)


.PHONY: test debug fun test-fun
