#include "files.h"

void init_display(WINDOW* scr) {
	current_screen.scr = scr;
	current_screen.max_x = getmaxx(scr);
	current_screen.max_y = getmaxy(scr);
	current_screen.cur_x = 0;
	current_screen.cur_y = 0;
	current_file.offset = 0;
}

void display() {
	char* content = load_content(&content);
	clear();
	printw(content);
	wmove(current_screen.scr, current_screen.cur_y, current_screen.cur_x);
	refresh();

	free(content);
}

void move_right() {
	char cur;
	int no_eof = pread(current_file.fd, &cur, 1, current_file.offset);
	if (no_eof) {
		if (cur == '\n') {
			current_screen.cur_x = 0;
			current_screen.cur_y++;					
		} else
			current_screen.cur_x++;
		current_file.offset++;
		wmove(current_screen.scr, current_screen.cur_y, current_screen.cur_x);
	} 
	refresh();
}

void move_left() {
	char ch;
	if (current_screen.cur_x != 0) {
		current_screen.cur_x--;
		current_file.offset--;
	} else {
		if (current_screen.cur_y != 0) {
			current_screen.cur_y--;
			current_file.offset -= 2;
			while (((pread(current_file.fd, &ch, 1, current_file.offset)) && (ch != '\n')) 
				&& (current_file.offset))
				current_file.offset--;
			if (current_file.offset)
				current_file.offset++;	
		}
	}
	wmove(current_screen.scr, current_screen.cur_y, current_screen.cur_x);
	refresh();
}

void k_press(int ch) {
	insert_char((char)ch, current_file.offset);
	if (ch != '\n') {
		current_screen.cur_x++;
		current_file.offset++;
	} else {
		current_file.offset++;
		current_screen.cur_x = 0;
		current_screen.cur_y++;
	}
	display();
}

void backspace() {
	if (current_file.offset) {
		delete_char(current_file.offset - 1);
		move_left();
		display();
	}
}

int menu() {
	noecho();
	int ch;
	while ((ch = wgetch(stdscr)) != ':') {
		switch(ch) {
		case KEY_LEFT:
			move_left();
			break;
		case KEY_RIGHT:
			move_right();
			break;
		case KEY_BACKSPACE:
			backspace();
			break;
		default:
			k_press(ch);
			break;
		}
	}
	switch (ch = wgetch(stdscr)) {
		case 'q':
			clear();
			refresh();
			return 0;
		case 's':
			save();
			return menu();
		default:
			return menu();

	}

}