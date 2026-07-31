#pragma once

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct game
{
	int 	height;
	int		width;
	int 	iteration;
	char	**map;
} t_game; 
