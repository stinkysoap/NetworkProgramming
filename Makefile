
CC = gcc
CFLAGS = -Wall -pthread

all: server receiver

server: server.o
	$(CC) $(CFLAGS) -o server server.o

receiver: receiver.o
	$(CC) $(CFLAGS) -o receiver receiver.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o server receiver

