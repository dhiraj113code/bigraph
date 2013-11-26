
CC = gcc
CFLAGS = -g

all:  sim

sim:  main.o biconn.o
	$(CC) -o sim main.o biconn.o

main.o:  main.c biconn.h
	$(CC) $(CFLAGS) -c main.c

cache.o:  biconn.c biconn.h
	$(CC) $(CFLAGS) -c biconn.c

clean:
	rm *.o sim

