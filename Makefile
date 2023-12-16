CC = gcc
CFLAGS = -Wall -Wextra -std=c99 $(shell sdl2-config --cflags)
LIBS = $(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf

SRC = main.c fonctions_fichiers.c
OBJ = $(SRC:.c=.o)

EXECUTABLE = main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJ)
