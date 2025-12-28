TARGET_SERVER = hw3server
TARGET_CLIENT = hw3client
SOURCE_SERVER = $(wildcard server/src/*.c)
INCLUDE_SERVER = $(wildcard client/include/*.h)
SOURCE_CLIENT = $(wildcard client/src/*.c)
INCLUDE_CLIENT = $(wildcard client/include/*.h)
CC = gcc
FLAGS = -Wall -Wextra
DEBUG_FLAGS = -Wall -Wextra -g -O0 -DDEBUG -fanalyzer -fsanitize=address

all: server client

debug: server-debug client-debug

server: $(SOURCE_SERVER) $(INCLUDE_SERVER)
	@$(CC) $(FLAGS) $(SOURCE_SERVER) -o $(TARGET_SERVER)

server-debug: $(SOURCE_SERVER) $(INCLUDE_SERVER)
	@$(CC) $(DEBUG_FLAGS)  $(SOURCE_SERVER) -o $(TARGET_SERVER)

client: $(SOURCE_CLIENT) $(INCLUDE_CLIENT)
	@$(CC) $(FLAGS) $(SOURCE_CLIENT) -o $(TARGET_CLIENT)

client-debug: $(SOURCE_CLIENT) $(INCLUDE_CLIENT)
	@$(CC) $(DEBUG_FLAGS) $(SOURCE_CLIENT) -o $(TARGET_CLIENT)
	
clean:
	@rm -f $(TARGET_SERVER) $(TARGET_CLIENT)	

.PHONY: clean
