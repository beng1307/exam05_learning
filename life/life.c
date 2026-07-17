#include "life.h"

static void	free_map(t_game *game)
{
	if (game->map)
	{
		for (int y = 0; y < game->height; y++)
			if (game->map[y])
				free(game->map[y]);
		free(game->map);
	}
}

static void	print_map(t_game game)
{
	for (int y = 0; y < game.height; y++)
	{
		for (int x = 0; x < game.height; x++)
			putchar(game.map[y][x]);
		putchar('\n');
	}
}

static void	fill_map(t_game *game)
{
	char	buffer;
	int		x = 0;
	int		y = 0;
	bool	draw = false;

	while(read(STDIN_FILENO, &buffer, 1) == 1)
	{
		switch (buffer)
		{
			case ('w'): if (y > 0) { --y; }; break ; 
			case ('a'): if (x > 0) { --x; }; break ; 
			case ('s'): if (y < game->height - 1) { ++y; }; break ;
			case ('d'): if (x < game->width - 1) { ++x; }; break ;
			case ('x'): draw = !draw; break ;
			default: continue;
		}

		if (draw && x >= 0 && x < game->width && y >= 0 && y <= game->height)
			game->map[y][x] = '0';
	}
}

static void	init_game(t_game *game, char **av)
{
	game->width = atoi(av[1]);
	game->height = atoi(av[2]);
	game->iteration = atoi(av[3]);

	game->map = malloc(game->height * sizeof(char *));
	for (int i = 0; i < game->height; i++)
	{
		game->map[i] = malloc(game->width * sizeof(char *));
		for (int j = 0; j < game->width; j++)
			game->map[i][j] = ' ';
	}

}

static int	count_neighbors(t_game *game, int y, int x)
{
	int count = 0;

	for (int dy = -1; dy < 2; dy++)
	{
		for (int dx = -1; dx < 2; dx++)
		{
			if (dx == 0 && dy == 0)
				continue ;

			int nx = x + dx;
			int ny = y + dy;
			if (nx >= 0 && nx < game->width && ny >= 0 && ny < game->height && game->map[ny][nx] == '0')
				count++;	
		}		
	}
	return (count);
}

static void	play_game(t_game *game)
{
	char **new_map = malloc(game->height * sizeof(char *));
	for (int i = 0; i < game->height; i++)
		new_map[i] = malloc(game->width * sizeof(char *));


	for (int y = 0; y < game->height; y++)
	{
		for (int x = 0; x < game->height; x++)
		{	
			int neighbors = count_neighbors(game, y, x);
			if (game->map[y][x] == '0')
			{
				if (neighbors == 2 || neighbors == 3)
					new_map[y][x] = '0';
				else
					new_map[y][x] = ' ';
			}	
			else
			{
				if (neighbors == 3)
					new_map[y][x] = '0';
				else
					new_map[y][x] = ' ';
			}
		}	
	}
	free_map(game);
	game->map = new_map;
}

int main(int ac, char **av)
{
	if (ac != 4)
		return (1);

	t_game game;

	init_game(&game, av);
	fill_map(&game);

	for (int i = 0; i < game.iteration; i++)
		play_game(&game);

	print_map(game);
	free_map(&game);
}