#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>

#include "files.h"

#define NAME_SIZE 200

int main(int argc, char** argv) {
	char* filename = (char*)malloc(sizeof(char) * NAME_SIZE);
	if (argc > 1)
		strcpy(filename, argv[1]);
	else {
		fprintf(stderr, "No input file.");
		exit(EXIT_FAILURE);
	}
	if (init_file(filename) != 0) {
		fprintf(stderr, "Error loading file");
		exit(EXIT_FAILURE);
	}	
	WINDOW* stdscr = initscr();
	init_display(stdscr);
	keypad(stdscr, TRUE);
	display();	
		
	menu();

	close_file();
	return 0;
}