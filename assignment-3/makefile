TARGET_SERVER = hw3server
TARGET_CLIENT = hw3client
SOURCE_SERVER = $(wildcard server/src/*.c server/include/*.h)
SOURCE_CLIENT = $(wildcard client/src/*.c client/include/*.h)
CC = gcc
FLAGS = -Wall -Wextra
DEBUG_FLAGS = -Wall -Wextra -g -O0 -DDEBUG -fanalyzer -fsanitize=address

# Default rule: build the program

server: $(SOURCE_SERVER)
	@$(CC) $(FLAGS) -o $(TARGET_SERVER) $(SOURCE_SERVER)

server-debug: $(SOURCE_SERVER)
	@$(CC) $(DEBUG_FLAGS) -o $(TARGET_SERVER) $(SOURCE_SERVER)

client: $(SOURCE_CLIENT)
	@$(CC) $(FLAGS) -o $(TARGET_CLIENT) $(SOURCE_CLIENT)

client-debug: $(SOURCE_CLIENT)
	@$(CC) $(DEBUG_FLAGS) -o $(TARGET_CLIENT) $(SOURCE_CLIENT)
	
clean:
	@rm -f $(TARGET_SERVER) $(TARGET_CLIENT)	

.PHONY: clean
