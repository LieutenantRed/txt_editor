#ifndef __FILES_H__
#define __FILES_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ncurses.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef BUFFER_SIZE 
#define BUFFER_SIZE 1024
#endif

struct {
	char* name;
	int fd;
	int size;
	size_t offset;
} current_file;

struct {
	WINDOW* scr;
	size_t cur_x, cur_y, max_x, max_y;
} current_screen;

// file handling
int init_file(const char* filename);
char* load_content(char** dest);
int insert_char(char ch, size_t poz);
int delete_char(size_t poz);
int close_file();


// file display
void init_display(WINDOW* scr);
void display();
int menu();
#endif