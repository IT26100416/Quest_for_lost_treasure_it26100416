#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_SIZE 15
#define WALL '#'  // wall symbol
#define EMPTY ' ' // empty space

char map[MAP_SIZE][MAP_SIZE];

void fn_initializeMap()
{
	for (int r = 0; r < MAP_SIZE; r++)
	{
		for(int c = 0; c < MAP_SIZE; c++)
		{
			map[r][c] = EMPTY;
		}
	}

	for(int i = 0; i < MAP_SIZE; i++)
	{
		map[0][i] = WALL;
		map[MAP_SIZE - 1][i] = WALL;
		map[i][0] = WALL;
		map[i][MAP_SIZE - 1] = WALL; 
	}

	int placed = 0;
	while (placed < 30)
	{
		int r = rand() % MAP_SIZE;
		int c = rand() % MAP_SIZE;

		if(map[r][c] == EMPTY && r > 0 && r < MAP_SIZE - 1 && c > 0 && c < MAP_SIZE - 1)
		{
			map[r][c] = WALL;
			placed++;
		}
	}
}

void fn_printMap()
{
	for(int r = 0; r < MAP_SIZE; r++)
	{
		for(int c = 0; c < MAP_SIZE; c++)
		{
			printf("%c ", map[r][c]);
		}
		printf("\n");
	}
}

int main()
{
	srand(time(NULL));
	fn_initializeMap();
	fn_printMap();
	return 0;
}

