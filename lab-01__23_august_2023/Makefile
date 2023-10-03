CC = gcc
CFLAGS = -Wall -Werror

all: client.out server.out

client.out: client.cpp
	$(CC) $(CFLAGS) client.cpp -o client.out

server.out: server.cpp
	$(CC) $(CFLAGS) server.cpp -o server.out

run_client: client.out
	./client.out 50 x 10

run_server: server.out
	./server.out

clear: client.out server.out
	rm -f client.out server.out

clean: client.out server.out
	rm -f client.out server.out
