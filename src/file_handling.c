#include "files.h"

int init_file(const char* filename) {
	current_file.name = (char*)malloc(sizeof(char) * BUFFER_SIZE);

	current_file.fd = open(filename, O_RDWR | O_CREAT, 0777);
	if (current_file.fd == -1) {
		free(current_file.name);
		fprintf(stderr, "Cannot open or create file.");
		exit(EXIT_FAILURE);	
	}

	current_file.size = lseek(current_file.fd, 0L, SEEK_END);
	return 0;
}

char* load_content(char** dest) {
	*dest = (char*)malloc(sizeof(char) * current_file.size);
	if (*dest == NULL) {
		fprintf(stderr, "Cannot allocate memory");
		exit(EXIT_FAILURE);
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
	pwrite(current_file.fd, tail, current_file.size - poz, poz);
	current_file.size = lseek(current_file.fd, 0L, SEEK_END);
	free(tail);
	return 0;
}

int close_file() {
	free(current_file.name);
	current_file.size = 0;
	close(current_file.fd);

	return 0;
}