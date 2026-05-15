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
int hiddenTrap[MAP_SIZE][MAP_SIZE]; // 1 = trap , 0 = no trap

int playerCount = 3;
int playerHealth[3];
int playerRow[3], playerCol[3];
int playerKeys[3];
int playerTreasure[3];

int totalTreasures = 12;

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
	while(placedTreasures < totalTreasures)
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

	//Place players
	for(int p = 0; p < playerCount; p++)
	{
		int r,c;
		do
		{
			r = rand() % MAP_SIZE;
			c = rand() % MAP_SIZE;
		}while (map[r][c] != EMPTY);

		playerRow[p] = r;
		playerCol[p] = c;
		playerHealth[p] = 10;
		playerKeys[p] = 0;
		playerTreasure[p] = 0;

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
				if(playerRow[p] == r && playerCol[p] == c && playerHealth[p] > 0)
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
		for(int p = 0; p < playerCount; p++)
		{
			printf("Player %d -> HP:%d Keys:%d Treasure:%d\n", p+1, playerHealth[p], playerKeys[p], playerTreasure[p]);
		}
}

int fn_movePlayer(int index, char move)
{
	if(playerHealth[index] <= 0)
	{
		return 0;
	}

	int r = playerRow[index];
	int c = playerCol[index];

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
		return 0;
	}

	char tile = map[r][c];

	if(tile == WALL)
	{
		return 0;
	}

	if(tile == DOOR && playerKeys[index] == 0)
	{
		printf("Player %d: Door is locked! Need a key.\n\n", index+1);
		return 0;
	}

	playerRow[index] = r;
	playerCol[index] = c;

	if(tile == TREASURE)
	{
		playerTreasure[index]++;
		map[r][c] = EMPTY;
		printf("Player %d collected treasure!\n\n", index+1);
		if(playerTreasure[index] >= 5)
		{
			printf("Player %d WINS by collecting 5 treasures!\n", index+1);
			return 1;
		}
	}
	else if(tile == HEALTH)
	{
		playerHealth[index] += 2;
		map[r][c] = EMPTY;
		printf("Player %d picked up health!\n\n", index+1);
	}
	else if(tile == KEY)
	{
		playerKeys[index]++;
		map[r][c] = EMPTY;
		printf("Player %d picked up a key!\n\n", index+1);
	}
	else if(tile == DOOR && playerKeys[index] > 0)
	{
		playerKeys[index]--;
		map[r][c] = EMPTY;
		printf("Player %d unlocked a door!\n\n", index+1);
	}

	if(hiddenTrap[r][c] == 1)
	{
		playerHealth[index] -= 3;
		hiddenTrap[r][c] = 0;//trap triggered
		printf("Player %d stepped on a trap!\n\n", index+1);
	}
	return 0;//No win yet
}

int main()
{
	srand(time(NULL)); // random seed
	fn_initializeMap();
	fn_printMap();

	char move;
	int currentPlayer = 0;
	int gameOver = 0;
	while (!gameOver)
	{
		if(playerHealth[currentPlayer] > 0)
		{
			printf("\nPlayer %d's turn (W/A/S/D, Q to quit): ", currentPlayer+1);
			scanf(" %c", &move);
			if(move == 'Q' || move == 'q')
			{
				break;
			}
			if(fn_movePlayer(currentPlayer, move))
			{
				gameOver = 1;
			}	
			fn_printMap();
		}
		currentPlayer = (currentPlayer + 1) % playerCount;//next player
	}	
	printf("\nGame Over!\n");
	return 0;
}

