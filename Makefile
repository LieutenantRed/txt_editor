all: main files
	gcc -Wall -o run main.o file_handling.o file_display.o -lncurses
	rm *.o

main: src/main.c
	gcc -Wall -o main.o -c src/main.c -g

files: src/files.h src/file_handling.c src/file_display.c
	gcc -Wall -o file_handling.o -c src/file_handling.c -g
	gcc -Wall -o file_display.o -c src/file_display.c -g


clear:
	rm run