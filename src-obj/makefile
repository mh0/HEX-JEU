.PHONY: all clean maxclean

CC=gcc
CFLAGS= -Wall $(shell sdl-config --cflags)
LDFLAGS= -lm $(shell sdl-config --libs)
SDLFLAGS= -lSDL -lSDL_image -lSDL_ttf 
EXEC=../hex_game
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

-include $(OBJ:.o=.d)

all:	$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC) $(LDFLAGS)  $(SDLFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
	$(CC) -MM $(CFLAGS) $*.c > $*.d

clean:
	rm -f $(OBJECTS)

maxclean: clean
	  rm -rf *.o *.d

