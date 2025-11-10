# Name of the executable
TARGET = hw1shell
SOURCE = src/*
# Default rule: build the program
$(TARGET): $(SOURCE)
	$(CC) -o $(TARGET) $(SOURCE)

# Run rule: depends on the binary, then executes it
run: $(TARGET)
	@./$(TARGET)

.PHONY: run
