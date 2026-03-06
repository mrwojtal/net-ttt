CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I lib/inc

BUILD_DIR = build

all: client_app server_app

client_app: client/client.c lib/src/game.c lib/inc/game.h lib/inc/globals.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/client_app client/client.c lib/src/game.c

server_app: server/server.c lib/src/game.c lib/inc/game.h lib/inc/globals.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/server_app server/server.c lib/src/game.c

clean:
	rm -rf $(BUILD_DIR)