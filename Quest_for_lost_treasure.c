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

//Colours
#define RESET "\x1b[0m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define BOLD "\x1b[1;37m"

char map[MAP_SIZE][MAP_SIZE];
int hiddenTrap[MAP_SIZE][MAP_SIZE]; // 1 = trap , 0 = no trap

int playerCount = 3;
int playerHealth[3];
int playerRow[3], playerCol[3];
int playerKeys[3];
int playerTreasure[3];

int totalTreasures = 12;
int numOfTreasuresToWin = 5;

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

		playerRow[p] 	= r;
		playerCol[p] 	= c;
		playerHealth[p] = 10;
		playerKeys[p] 	= 0;
		playerTreasure[p] = 0;
	}
}

void fn_printTile(char tile)
{
	if(tile == TREASURE)
	{
		printf(YELLOW "T " RESET);
	}
	else if(tile == HEALTH)
	{
		printf(GREEN "H " RESET);
	}
	else if(tile == KEY)
	{
		printf(BLUE "K " RESET);
	}
	else if(tile == DOOR)
	{
		printf(RED "D " RESET);
	}
	else 
	{
		printf("%c ", tile);
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
					printf(BOLD "%d "RESET, p+1);
					printed = 1;
					break;
				}
			}
			if(!printed)
			{
				fn_printTile(map[r][c]);
			}
		}
		printf("\n");
	}
		for(int p = 0; p < playerCount; p++)
		{
			printf("Player %d -> HP:%d | Keys:%d | Treasure:%d\n", p+1, playerHealth[p], playerKeys[p], playerTreasure[p]);
		}
}

void fn_saveGame(const char *filename)
{
	FILE *f = fopen(filename, "w");
	if(!f)
	{
		printf("Error saving game!\n");
		return;
	}

	//save map
	for(int r = 0; r < MAP_SIZE; r++)
	{
		for(int c = 0; c < MAP_SIZE; c++)
		{
			fputc(map[r][c], f);
		}
		fputc('\n', f);
	}

	//save traps
	for(int r = 0; r < MAP_SIZE; r++)
	{
		for(int c = 0; c < MAP_SIZE; c++)
		{
			fprintf(f, "%d ", hiddenTrap[r][c]);
		}
		fprintf(f, "\n");
	}

	//Save players
	for(int p = 0; p < playerCount; p++)
	{
		fprintf(f, "%d %d %d %d %d\n", playerRow[p], playerCol[p], playerHealth[p], playerKeys[p], playerTreasure[p]);
	}
	fclose(f);
	printf("Game saved to %s\n", filename);	
}

void fn_loadGame(const char *filename)
{
	FILE *f = fopen(filename, "r");
	if(!f)
	{
		printf("Error loading game!\n");
		return;
	}

	//Load map
	for(int r = 0; r < MAP_SIZE; r++)
	{
		for(int c = 0; c < MAP_SIZE; c++)
		{
			fscanf(f, "%c", &map[r][c]);
		}
		fgetc(f);//newline
	}

	//Load traps
	for(int r = 0; r < MAP_SIZE; r++)
	{
		for(int c = 0; c < MAP_SIZE; c++)
		{
			fscanf(f, "%d ", &hiddenTrap[r][c]);
		}
	}

	//Load players
	for(int p = 0; p < playerCount; p++)
	{
		if(fscanf(f, "%d %d %d %d %d\n", &playerRow[p], &playerCol[p], &playerHealth[p], &playerKeys[p], &playerTreasure[p])!=5)
		{
			printf("Error reading player %d data\n", p+1);
		}
	}
	fclose(f);
	printf("Game loaded from %s\n", filename);	
}

void fn_updateLeaderboard(int winner, int score)
{
	FILE *f = fopen("leaderboard.txt", "a");
	if(!f)
	{
		printf("Error updating leaderboard!\n");
		return;
	}
	fprintf(f, "Player %d won with score %d\n", winner+1, score);
	fclose(f);
	printf("Leaderboard updated!\n");
}

void fn_showLeaderboard()
{
	FILE *f = fopen("leaderboard.txt", "r");
	if(!f)
	{
		printf("No leaderboard yet.\n");
		return;
	}
	printf("   === Leaderboard ===\n");
	char line[256];
	while (fgets(line, sizeof(line), f))
	{
		printf("%s", line);
	}
	fclose(f);
	printf("   ===================\n");
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
		if(playerTreasure[index] >= numOfTreasuresToWin)
		{
			printf("Player %d WINS by collecting %d treasures!\n", index+1, numOfTreasuresToWin);
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
	
	fn_showLeaderboard();

	char choice;
	printf("Start new game (N) or load game (L)? ");
	scanf(" %c", &choice);

	if(choice == 'L' || choice == 'l')
	{
		fn_loadGame("savegame.txt");
	}
	else
	{
		fn_initializeMap();
	}
 
	char move;
	int currentPlayer = 0;
	int gameOver = 0;

	while (!gameOver)
	{
		if(playerHealth[currentPlayer] > 0)
		{
			fn_printMap();

			printf(GREEN "\n    >> Player %d's turn <<\n" RESET, currentPlayer+1);
			printf("(W/A/S/D)-move, Q-quit, E-save: ");

			scanf(" %c", &move);
			if(move == 'Q' || move == 'q')
			{
				break;
			}
			if(move == 'E' || move == 'e')
			{
				fn_saveGame("savegame.txt");
			}
			else
			{
				if(fn_movePlayer(currentPlayer, move))
				{
					fn_updateLeaderboard(currentPlayer, playerTreasure[currentPlayer]);
					gameOver = 1;
				}
			}
	
		}
		currentPlayer = (currentPlayer + 1) % playerCount;//next player
	}	
	printf("\nGame Over!\n");

	return 0;
}

