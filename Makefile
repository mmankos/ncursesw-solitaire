# variables
CC=gcc
CFLAGS=-std=c11 -Wall -pedantic -Werror -Wno-vla-parameter
LDLIBS=-lm -lncursesw -D_GNU_SOURCE -D_DEFAULT_SOURCE
OUTPUT=solitaire

# targets
all: $(OUTPUT)

$(OUTPUT): game.o actions.o draw.o solitaire.o
	$(CC) $(CFLAGS) game.o actions.o draw.o solitaire.o $(LDLIBS) -o $(OUTPUT)

game.o: game.c game.h
	$(CC) $(CFLAGS) -c game.c $(LDLIBS) -o game.o

solitaire.o: solitaire.c
	$(CC) $(CFLAGS) -c solitaire.c $(LDLIBS) -o solitaire.o

actions.o: actions.c actions.h
	$(CC) $(CFLAGS) -c actions.c $(LDLIBS) -o actions.o

draw.o: draw.c draw.h
	$(CC) $(CFLAGS) -c draw.c $(LDLIBS) -o draw.o

# remove compiled files
clean:
	rm -rf $(OUTPUT) *.o
