CC = gcc
CFLAGS = -Wall -Wextra -std=c99 `sdl2-config --cflags`
LIBS = `sdl2-config --libs` -lSDL2_image

main: main.c fonctions_fichiers.c
	$(CC) $(CFLAGS) -o main main.c fonctions_fichiers.c $(LIBS)

clean:
	rm -f main

