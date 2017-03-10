CC = gcc
objects = Logger.o main.o
cfiles = Logger.c main.c 
headers = Logger.h
name = logger

logger : $(objects)
	$(CC) -o $(name) $(objects) -lm

main.o : main.c $(headers)
	$(CC) -c main.c

Logger.o : Logger.c
	$(CC) -c Logger.c

debug : $(cfiles)
	$(CC) -g $(cfiles) -lm -o $(name)

clean:
	rm $(name) $(objects)