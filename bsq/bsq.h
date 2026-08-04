#ifndef BSQ_H
#define BSQ_H

#include <stdio.h>

typedef struct s_map
{
	int rows;
	int cols;
	char empty;
	char obstacle;
	char full;
	char **grid;
}   t_map;

int process_stream(FILE *stream);
int process_file(const char *path);

#endif
