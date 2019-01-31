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
		free(filename);
		exit(EXIT_FAILURE);
	}

	if (init_file(filename) != 0) {
		fprintf(stderr, "Error loading file");
		free(filename);
		exit(EXIT_FAILURE);
	}	

	WINDOW* stdscr = initscr();
	init_display(stdscr);
	keypad(stdscr, TRUE);
	printw(":q to exit, :s to save");
	getch();
	display();	
		
	menu();
	
	endwin();
	close_file();	
	free(filename);
	return 0;
}