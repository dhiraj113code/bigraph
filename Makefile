
CC = gcc
CFLAGS = -g

all:  sim

sim:  main.o 
	$(CC) -o sim main.o 

main.o:  main.c 
	$(CC) $(CFLAGS) -c main.c

clean:
	rm *.o sim

