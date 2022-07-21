#include "model.h"

//Converts a CSV file to a 2D int array
void parse(struct room* storage, char* path)
{
	struct stat stats;
	stat(path, &stats);
	FILE* f = fopen(path, "r");
	assert(f); //to check if the path is correct
	char buf[MAX_ROOM_HEIGHT * MAX_ROOM_LENGTH * 2 + MAX_COMMENT_LENGTH]; // * 2 because of commas
	fread(buf, sizeof(buf[0]), stats.st_size, f);
	fclose(f);

	int start_index = 0;
	if (buf[0] == '#') {
		for (start_index = 0; buf[start_index] != '\n' ; ++start_index)
			;
		++start_index; //next symbol after #bla-bla-bla\n
	}
	
	storage->height = 0;
	int x = 0;
	for (int i = start_index; i < stats.st_size; ++i) {
		if (buf[i] == '\n') {
			++storage->height;
			storage->length = x;
			x = 0;
			continue;
		}
		if (isdigit(buf[i])) {
			storage->data[storage->height][x] = buf[i] - '0';
			++x;
		}
	}

	printf("storage [%d, %d]\n", storage->height, storage->length);
	for (int y = 0; y < storage->height; ++y) {
		for (int x = 0; x < storage->length; ++x)
			printf("%d ", storage->data[y][x]);
		printf("\n");
	}
}


