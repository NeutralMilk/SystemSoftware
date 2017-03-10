CC = gcc
objects = obj/main.o obj/backup.o obj/timestamp.o obj/configuration.o obj/logger.o obj/permissions.o
cfiles =  main.c backup.c timestamp.c configuration.c logger.c permissions.c
headers = backup.h timestamp.h configuration.h logger.h permissions.h
name = myProg
OBJDIR = obj

myProg: $(objects)
	$(CC) -o $(name) $(objects) -lm

$(OBJDIR)/main.o : main.c $(headers)
	$(CC) -c main.c -o $(OBJDIR)/main.o

$(OBJDIR)/permissions.o : permissions.c $(headers)
	$(CC) -c permissions.c -o $(OBJDIR)/permissions.o

$(OBJDIR)/logger.o : logger.c $(headers)
	$(CC) -c logger.c -o $(OBJDIR)/logger.o

$(OBJDIR)/backup.o : backup.c
	$(CC) -c backup.c -o $(OBJDIR)/backup.o

$(OBJDIR)/timestamp.o : timestamp.c
	$(CC) -c timestamp.c -o $(OBJDIR)/timestamp.o

$(OBJDIR)/configuration.o : configuration.c
	$(CC) -c configuration.c -o $(OBJDIR)/configuration.o

debug: $(cfiles)
	$(CC) -g $(cfiles) -lm -o $(name)

clean:
	rm $(name) $(objects)