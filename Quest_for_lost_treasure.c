#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_SIZE 15
#define WALL '#'  // wall symbol
#define EMPTY ' ' // empty space
#define TREASURE 'T'
#define HEALTH 'H'
#define KEY 'K'
#define DOOR 'D'

char map[MAP_SIZE][MAP_SIZE];
int hiddenTrap[MAP_SIZE][MAP_SIZE];

int playerCount = 3;
int playerRow[3];
int playerCol[3];

void fn_initializeMap()
{
	//Fill with empty
	for (int r = 0; r < MAP_SIZE; r++)
	{
		for(int c = 0; c < MAP_SIZE; c++)
		{
			map[r][c] = EMPTY;
			hiddenTrap[r][c] = 0;
		}
	}

	//Borders
	for(int i = 0; i < MAP_SIZE; i++)
	{
		map[0][i] = WALL;
		map[MAP_SIZE - 1][i] = WALL;
		map[i][0] = WALL;
		map[i][MAP_SIZE - 1] = WALL; 
	}

	//Random interior walls
	int placedWalls = 0;
	while (placedWalls < 30)
	{
		int r = rand() % MAP_SIZE;
		int c = rand() % MAP_SIZE;
		if(map[r][c] == EMPTY && r > 0 && r < MAP_SIZE - 1 && c > 0 && c < MAP_SIZE - 1)
		{
			map[r][c] = WALL;
			placedWalls++;
		}
	}

	//Random treasures
	int placedTreasures  = 0;
	while(placedTreasures < 12)
	{
	
		int r = rand() % MAP_SIZE;
		int c = rand() % MAP_SIZE;
		if(map[r][c] == EMPTY)
		{

			map[r][c] = TREASURE;
			placedTreasures++;
		}
	}

	//Random health packs
	int placedHealth = 0;
	while (placedHealth < 5)
	{
		int r = rand() % MAP_SIZE;
		int c = rand() % MAP_SIZE;
		if(map[r][c] == EMPTY)
		{
			map[r][c] = HEALTH;
			placedHealth++;

		}

	}

	//Random keys
	int placedKeys = 0;
	while (placedKeys < 3)
	{
		int r = rand() % MAP_SIZE;
		int c = rand() % MAP_SIZE;
		if(map[r][c] == EMPTY)
		{
			map[r][c] = KEY;
			placedKeys++;
		}

	}
	
	// Random doors
	int placedDoors = 0;
	while (placedDoors < 3)
	{
		int r = rand() % MAP_SIZE;
		int c = rand() % MAP_SIZE;
		if(map[r][c] == EMPTY)
		{
			map[r][c] = DOOR;
			placedDoors++;
		}

	}

	//Hidden traps
	int placedTraps = 0;
	while (placedTraps < 10)
	{
		int r = rand() % MAP_SIZE;
		int c = rand() % MAP_SIZE;
		if(map[r][c] == EMPTY && hiddenTrap[r][c] == 0)
		{
			hiddenTrap[r][c] = 1;
			placedTraps++;
		}
	}

	//Place player
	for(int p = 0; p < MAP_SIZE; p++)
	{
		int r,c;
		do
		{
			r = rand() % MAP_SIZE;
			c = rand() % MAP_SIZE;
		}while (map[r][c] != EMPTY);

		playerRow[p] = r;
		playerCol[p] = c;
	}
}

void fn_printMap()
{
	for(int r = 0; r < MAP_SIZE; r++)
	{
		for(int c = 0; c < MAP_SIZE; c++)
		{
			char tile = map[r][c];
			int printed = 0;
			for(int p = 0; p < playerCount; p++)
			{
				if(playerRow[p] == r && playerCol[p] == c)
				{
					printf("%d ", p+1);
					printed = 1;
					break;
				}
			}
			if(!printed)
			{
				printf("%c ", tile);
			}
		}
		printf("\n");
	}
}

int main()
{
	srand(time(NULL)); // random seed
	fn_initializeMap();
	fn_printMap();
	return 0;
}

