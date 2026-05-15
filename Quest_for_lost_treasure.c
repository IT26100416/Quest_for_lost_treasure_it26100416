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

int playerHealth = 10;
int playerRow = 0, playerCol = 0;
int playerKeys  = 0;
int playerTreasure = 0;

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
		if(map[r][c] == EMPTY)
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

	//Place player 1
	int r,c;
		do
		{
			r = rand() % MAP_SIZE;
			c = rand() % MAP_SIZE;
		}while (map[r][c] != EMPTY);

		playerRow = r;
		playerCol = c;
}

void fn_printMap()
{
	for(int r = 0; r < MAP_SIZE; r++)
	{
		for(int c = 0; c < MAP_SIZE; c++)
		{
			char tile = map[r][c];
			if(playerRow == r && playerCol == c)
			{
				printf("P ");//player
			}
			else
			{
				printf("%c ", tile);
			}
		}
		printf("\n");
	}
	printf("Health: %d | Keys: %d | Treasure: %d\n", playerHealth, playerKeys, playerTreasure);
}

void fn_movePlayer(char move)
{
	int r = playerRow;
	int c = playerCol;

	if(move == 'W' || move == 'w')
	{
		r--;
	}
	else if(move == 'S' || move == 's')
	{
		r++;
	}
	else if(move == 'A' || move == 'a')
	{
		c--;
	}
	else if(move == 'D' || move == 'd')
	{
		c++;
	}
	//check if valid (not wall and outside)
	if(r < 0 || r >= MAP_SIZE || c < 0 || c >= MAP_SIZE)
	{
		return;
	}

	char tile = map[r][c];

	if(tile == WALL)
	{
		return;
	}

	if(tile == DOOR && playerKeys == 0)
	{
		printf("Door is locked! Need a key.\n");
		return;
	}

	playerRow = r;
	playerCol = c;

	if(tile == TREASURE)
	{
		playerTreasure++;
		map[r][c] = EMPTY;
		printf("Collected treasure! Total: %d\n", playerTreasure);
	}
	else if(tile == HEALTH)
	{
		playerHealth += 2;
		map[r][c] = EMPTY;
		printf("Picked up health! HP: %d\n", playerHealth);
	}
	else if(tile == KEY)
	{
		playerKeys++;
		map[r][c] = EMPTY;
		printf("Picked up a key! Keys: %d\n", playerKeys);
	}
	else if(tile == DOOR && playerKeys > 0)
	{
		playerKeys--;
		map[r][c] = EMPTY;
		printf("Unlocked a door! Keys left: %d\n", playerKeys);
	}

	if(hiddenTrap[r][c] == 1)
	{
		playerHealth -= 3;
		hiddenTrap[r][c] = 0;//trap triggered
		printf("Stepped on a trap! HP: %d\n", playerHealth);
	}
}

int main()
{
	srand(time(NULL)); // random seed
	fn_initializeMap();
	fn_printMap();

	char move;
	while (playerHealth > 0)
	{
		printf("Enter move (W/A/S/D, Q to quit): ");
		scanf(" %c", &move);
		if(move == 'Q' || move == 'q')
		{
			break;
		}
	fn_movePlayer(move);
	fn_printMap();
	}

	printf("Game Over!\n");
	return 0;
}

