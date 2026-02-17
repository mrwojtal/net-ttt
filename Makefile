CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I lib/inc

all: client_app server_app

client_app: client/client.c lib/src/game.c lib/inc/game.h lib/inc/globals.h
		$(CC) $(CFLAGS) -o client_app client/client.c lib/src/game.c

server_app: server/server.c lib/src/game.c lib/inc/game.h lib/inc/globals.h
		$(CC) $(CFLAGS) -o server_app server/server.c lib/src/game.c

clean:
		rm -f client_app server_app
