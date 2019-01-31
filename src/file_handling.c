#include "files.h"

int init_file(const char* filename) {
	
	// open tmp and orig
	char* tmp = (char*)malloc(sizeof(char) * BUFFER_SIZE);
	current_file.name = (char*)malloc(sizeof(char) * BUFFER_SIZE);
	{
		strcpy(current_file.name, filename);
		strcpy(tmp, current_file.name);
		strcat(tmp, ".tmp");
	}
	int file = open(current_file.name, O_RDWR | O_CREAT, 0666);
	current_file.fd = open(tmp, O_RDWR | O_CREAT, 0666);
	free(tmp);
	if ((current_file.fd == -1) || (file == -1)) {
		free(current_file.name);
		fprintf(stderr, "Cannot open or create file.");
		return -1;	
	}

	// copy orig file to tmp
	char c;
	while (read(file, &c, 1) != 0) {
		write(current_file.fd, &c, 1);
	}

	// fill struct
	current_file.size = lseek(current_file.fd, 0L, SEEK_END);
	lseek(current_file.fd, 0L, SEEK_SET);
	current_file.offset = 0;

	// close orig for a while
	close(file);

	// working with tmp fd
	return 0;
}

int save() {	// rewrite file
	remove(current_file.name);
	int file = open(current_file.name, O_RDWR | O_CREAT, 0666);
	char c;
	lseek(current_file.fd, 0L, SEEK_SET);
	while (read(current_file.fd, &c, 1) != 0) {
		if (c != '\0')
			write(file, &c, 1);
	}
	close(file);
	return 0;
}

char* load_content(char** dest) {
	*dest = (char*)malloc(sizeof(char) * current_file.size);
	if (*dest == NULL) {
		fprintf(stderr, "Cannot allocate memory");
		return NULL;
	}
	lseek(current_file.fd, 0L, SEEK_SET);
	read(current_file.fd, *dest, current_file.size);

	return *dest;
}

char* load_tail(size_t poz, char** dest) {
	if (poz > current_file.size)
		return NULL;
	*dest = (char*)malloc(sizeof(char) * (current_file.size - poz));
	
	lseek(current_file.fd, poz, SEEK_SET);
	read(current_file.fd, *dest, current_file.size - poz);
	
	return *dest;
}

int insert_char(char ch, size_t poz) {
	lseek(current_file.fd, 0L, SEEK_SET);
	char* tail = load_tail(poz, &tail);
	if (tail == NULL) {
		fprintf(stderr, "Cannot reach poz");
		exit(EXIT_FAILURE);
	}
	pwrite(current_file.fd, &ch, 1, poz);
	pwrite(current_file.fd, tail, current_file.size - poz, poz + 1);
	current_file.size = lseek(current_file.fd, 0L, SEEK_END);
	free(tail);
	return 0;
}

int delete_char(size_t poz) {
	lseek(current_file.fd, 0L, SEEK_SET);
	char* tail = load_tail(poz + 1, &tail);
	if (tail == NULL) {
		fprintf(stderr, "Cannot reach poz");
		exit(EXIT_FAILURE);
	}
	int num = strlen(tail);
	pwrite(current_file.fd, tail, num + 1, poz);
	pwrite(current_file.fd, "\0", 1, current_file.size - 1);
	current_file.size = lseek(current_file.fd, 0L, SEEK_END);
	free(tail);
	return 0;
}

int close_file() {
	//remove tmp file
	char* tmp = (char*)malloc(sizeof(char) * BUFFER_SIZE);
	strcpy(tmp, current_file.name);
	strcat(tmp, ".tmp");
	remove(tmp);
	free(tmp);

	// free struct
	free(current_file.name);
	current_file.size = 0;
	close(current_file.fd);

	return 0;
}