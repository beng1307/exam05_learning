#include "bsq.h"

#include <errno.h>
#include <stdlib.h>

/* Printable ASCII range used by the subject. */
static int is_printable(char c)
{
	return (c >= 32 && c <= 126);
}

/* Read one positive integer from a string (manual parser, no atoi). */
static int parse_positive_int(const char *s, int *index, int *out)
{
	long value;
	int i;

	i = *index;
	value = 0;
	if (!(s[i] >= '0' && s[i] <= '9'))
		return (0);
	while (s[i] >= '0' && s[i] <= '9')
	{
		value = value * 10 + (s[i] - '0');
		if (value > 2147483647)
			return (0);
		i++;
	}
	*index = i;
	*out = (int)value;
	return (1);
}

static void skip_spaces(const char *s, int *index)
{
	while (s[*index] == ' ')
		(*index)++;
}

/* Header format: "rows empty obstacle full". */
static int parse_header_line(char *line, t_map *map)
{
	int i;

	i = 0;
	skip_spaces(line, &i);
	if (!parse_positive_int(line, &i, &map->rows))
		return (0);
	if (map->rows <= 0)
		return (0);
	if (line[i] != ' ')
		return (0);
	skip_spaces(line, &i);
	if (line[i] == '\0' || line[i] == '\n')
		return (0);
	map->empty = line[i++];
	if (line[i] != ' ')
		return (0);
	skip_spaces(line, &i);
	if (line[i] == '\0' || line[i] == '\n')
		return (0);
	map->obstacle = line[i++];
	if (line[i] != ' ')
		return (0);
	skip_spaces(line, &i);
	if (line[i] == '\0' || line[i] == '\n')
		return (0);
	map->full = line[i++];
	skip_spaces(line, &i);
	if (!(line[i] == '\n' || line[i] == '\0'))
		return (0);
	if (!is_printable(map->empty) || !is_printable(map->obstacle)
		|| !is_printable(map->full))
		return (0);
	if (map->empty == map->obstacle || map->empty == map->full
		|| map->obstacle == map->full)
		return (0);
	return (1);
}

static void free_map(t_map *map)
{
	int i;

	if (!map->grid)
		return ;
	i = 0;
	while (i < map->rows)
	{
		free(map->grid[i]);
		i++;
	}
	free(map->grid);
	map->grid = NULL;
}

static int alloc_rows(t_map *map)
{
	int i;

	map->grid = (char **)calloc((size_t)map->rows, sizeof(char *));
	if (!map->grid)
		return (0);
	i = 0;
	while (i < map->rows)
	{
		map->grid[i] = NULL;
		i++;
	}
	return (1);
}

static int store_row(t_map *map, int row_idx, const char *line, int width)
{
	int j;

	map->grid[row_idx] = (char *)malloc((size_t)width + 1);
	if (!map->grid[row_idx])
		return (0);
	j = 0;
	while (j < width)
	{
		map->grid[row_idx][j] = line[j];
		j++;
	}
	map->grid[row_idx][width] = '\0';
	return (1);
}

static int validate_map_row(const t_map *map, const char *line, int width)
{
	int j;

	j = 0;
	while (j < width)
	{
		if (line[j] != map->empty && line[j] != map->obstacle)
			return (0);
		j++;
	}
	return (1);
}

/*
** Read exactly map->rows lines.
** Every line must:
** - end with '\n'
** - have same width
** - contain only empty/obstacle chars
** Also reject extra content after the map.
*/
static int read_map_rows(FILE *stream, t_map *map)
{
	char *line;
	size_t cap;
	size_t len;
	int row;
	int width;

	line = NULL;
	cap = 0;
	row = 0;
	width = -1;
	while (row < map->rows)
	{
		len = getline(&line, &cap, stream);
		if (len <= 0 || line[len - 1] != '\n')
		{
			free(line);
			return (0);
		}
		if (width == -1)
		{
			width = (int)(len - 1);
			if (width <= 0)
			{
				free(line);
				return (0);
			}
			map->cols = width;
		}
		else if (width != (int)(len - 1))
		{
			free(line);
			return (0);
		}
		if (!validate_map_row(map, line, width) || !store_row(map, row, line, width))
		{
			free(line);
			return (0);
		}
		row++;
	}
	len = getline(&line, &cap, stream);
	free(line);
	if (len != -1)
		return (0);
	return (1);
}

static int min3(int a, int b, int c)
{
	int m;

	m = a;
	if (b < m)
		m = b;
	if (c < m)
		m = c;
	return (m);
}

static void fill_best_square(t_map *map, int best_size, int best_i, int best_j)
{
	int i;
	int j;
	int start_i;
	int start_j;

	if (best_size <= 0)
		return ;
	start_i = best_i - best_size + 1;
	start_j = best_j - best_size + 1;
	i = start_i;
	while (i <= best_i)
	{
		j = start_j;
		while (j <= best_j)
		{
			map->grid[i][j] = map->full;
			j++;
		}
		i++;
	}
}

/*
** Dynamic programming:
** dp[i][j] = biggest square size ending at cell (i, j).
** If current cell is empty:
**   dp[i][j] = 1 + min(top, left, top-left)
** Else (obstacle):
**   dp[i][j] = 0
*/
static int solve_map(t_map *map)
{
	int i;
	int j;
	int best_size;
	int best_i;
	int best_j;
	int **dp;

	dp = (int **)calloc((size_t)map->rows, sizeof(int *));
	if (!dp)
		return (0);
	i = 0;
	while (i < map->rows)
	{
		dp[i] = (int *)calloc((size_t)map->cols, sizeof(int));
		if (!dp[i])
		{
			while (i-- > 0)
				free(dp[i]);
			free(dp);
			return (0);
		}
		i++;
	}
	best_size = 0;
	best_i = 0;
	best_j = 0;
	i = 0;
	while (i < map->rows)
	{
		j = 0;
		while (j < map->cols)
		{
			if (map->grid[i][j] == map->obstacle)
				dp[i][j] = 0;
			else if (i == 0 || j == 0)
				dp[i][j] = 1;
			else
				dp[i][j] = 1 + min3(dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]);
			if (dp[i][j] > best_size)
			{
				best_size = dp[i][j];
				best_i = i;
				best_j = j;
			}
			j++;
		}
		i++;
	}
	fill_best_square(map, best_size, best_i, best_j);
	i = 0;
	while (i < map->rows)
	{
		free(dp[i]);
		i++;
	}
	free(dp);
	return (1);
}

static void print_map(const t_map *map)
{
	int i;

	i = 0;
	while (i < map->rows)
	{
		fprintf(stdout, "%s\n", map->grid[i]);
		i++;
	}
}

/* Process one map source: parse, validate, solve, print or error. */
int process_stream(FILE *stream)
{
	t_map map;
	char *line;
	size_t cap;
	size_t len;

	map.rows = 0;
	map.cols = 0;
	map.empty = 0;
	map.obstacle = 0;
	map.full = 0;
	map.grid = NULL;
	line = NULL;
	cap = 0;
	len = getline(&line, &cap, stream);
	if (len <= 0 || line[len - 1] != '\n' || !parse_header_line(line, &map)
		|| !alloc_rows(&map) || !read_map_rows(stream, &map) || !solve_map(&map))
	{
		free(line);
		free_map(&map);
		fprintf(stderr, "map error\n");
		return (1);
	}
	free(line);
	print_map(&map);
	free_map(&map);
	return (0);
}

int process_file(const char *path)
{
	FILE *file;
	int result;

	errno = 0;
	file = fopen(path, "r");
	if (!file)
	{
		fprintf(stderr, "map error\n");
		return (1);
	}
	result = process_stream(file);
	fclose(file);
	return (result);
}

/*
** - no argument: read one map from stdin
** - arguments: each argument is one map file
** - print one blank line between outputs
*/
int main(int argc, char **argv)
{
	int i;

	if (argc == 1)
		return (process_stream(stdin));
	i = 1;
	while (i < argc)
	{
		process_file(argv[i]);
		if (i < argc - 1)
			fprintf(stdout, "\n");
		i++;
	}
	return (0);
}
