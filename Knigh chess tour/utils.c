#include "utils.h"

/*this function recives a pointer of allocated memory and checks if a memory allocation failed. exits the program if it failed*/
void checkMemoryAllocation(void* ptr) {
	if (ptr == NULL)
	{
		printf("[-] Memory Allocation Error!!!");
		exit(1);
	}
}

void freeValidKnightMoves(chessPosArray*** valid_moves) {
	int i, j;
	for (i = 0; i < BOARD_LENGTH; i++)
	{
		for (j = 0; j < BOARD_LENGTH; j++)
		{
			free(valid_moves[i][j]->positions);
			free(valid_moves[i][j]);
		}
		free(valid_moves[i]);
	}

}

/*this function calculates for each cell on the board the possible moves a knight can do.
the function returns the possible moves for each cell on the board a knight can do
and how many moves are possible for the knight from each cell.*/
chessPosArray*** validKnightMoves()
{
	int i, j;
	
	static chessPosArray*** valid_moves = NULL;
	if (valid_moves != NULL)
		return valid_moves;

	valid_moves = (chessPosArray***)malloc(BOARD_LENGTH * sizeof(chessPosArray**));
	checkMemoryAllocation(valid_moves);

	for (i = 0; i < BOARD_LENGTH; i++)
	{
		valid_moves[i] = (chessPosArray**)malloc(BOARD_LENGTH * sizeof(chessPosArray*));
		checkMemoryAllocation(valid_moves[i]);

		for (j = 0; j < BOARD_LENGTH; j++)
			valid_moves[i][j] = LegitMoves(i, j);
	}
	return valid_moves;
}

/* recives an row and column as an index.
the function returns an chessPosArray which contians the numbers of possible moves a knight can do from the current index
and their locations */
chessPosArray* LegitMoves(int row, int col)
{
	int size = 0;
	chessPosArray* knight = (chessPosArray*)malloc(sizeof(chessPosArray));
	checkMemoryAllocation(knight);
	knight->positions = Moves(row, col, &size);
	knight->size = size;
	return knight;
}

/*this function recives the index of an row and colomn and a output parameter size.
the function calculates and returns the possibles moves a knight can do from the current index (recived by row and colomn)
it also updates the size of those possible moves.*/
chessPos* Moves(int row, int col, int* size)
{
	int move_row, move_col;
	int i, j, currMoveIndex = 0;
	chessPos* moves = (chessPos*)malloc(8 * sizeof(chessPos)); // every knight maximum steps is 8.
	checkMemoryAllocation(moves);

	for (i = -2; i <= 2; i++)
	{
		if (i != 0)
			move_row = row + i;
		else // a knight cant move to the same row is currently on.
			continue;
		for (j = 0; j < 2; j++)
		{
			if (i % 2 == 0)
				if (j == 1)
					move_col = col + 1;
				else
					move_col = col - 1;
			else
				if (j == 1)
					move_col = col + 2;
				else
					move_col = col - 2;

			if (isLegalIndex(move_row) && isLegalIndex(move_col))
			{
				moves[currMoveIndex][0] = IntToCharRow(move_row);
				moves[currMoveIndex][1] = IntToCharCol(move_col);
				currMoveIndex++;
			}
		}
	}

	if (currMoveIndex < BOARD_LENGTH)
	{
		moves = realloc(moves, currMoveIndex * sizeof(chessPos));
		checkMemoryAllocation(moves);
	}
	*size = currMoveIndex;
	return moves;
}

/*this function receives two chars representing row and colomn.
the function checks if the current location is legal (if the location is actually on the board)*/
bool isLegalIndex(int index)
{
	if (index >= 0 && index < BOARD_LENGTH)
		return true;
	return false;
}

/*this function receives a number and returns its representaion as a capital letter*/
char IntToCharRow(int num)
{
	if (isLegalIndex(num))
		return num + 'A';
	else {
		printf("[!] Unexpected index, in IntToCharRow  num: %d is not between 0-8", num);
		exit(3);
	}
}

/*this function receives a char representing an index, and returns its numeric value*/
int charIndexToInt(char c) {
	if (c >= 'A' && c <= ('A' + BOARD_LENGTH))
		return c - 'A';
	else if (c >= '1' && c <= '0' + BOARD_LENGTH)
		return c - '0' - 1;
	else {
		printf("[!] Error, unexpected char index in charIndexToInt, char: %c", c);
		exit(1);
	}
}

/*this function receives a number and returns its representaion as a char*/
char IntToCharCol(int num)
{
	if (isLegalIndex(num))
		return num + '0' + 1;
	else {
		printf("[!] Unexpected index, in IntToCharCol convert num: %d is not between 0-8", num);
		exit(1);
	}
}

/*this function recives a char column and checks if its position is legal. returns the result of the check*/
bool isLegalColChar(char col) {
	if (col >= '1' && col <= ('1' + BOARD_LENGTH - 1))
		return true;
	return false;
}

/*this function recives a char row and checks if its position is legal. returns the result of the check*/
bool isLegalRowChar(char row) {
	if (row >= 'A' && row <= ('A' + BOARD_LENGTH - 1))
		return true;
	return false;
}

/*this function recives a position and prints it.*/
void printChessPosition(chessPos position) {
	printf("(%c,%c)", position[0], position[1]);
}

/* this function gets a user input from the user, creates a string out of the input and returns it.*/
char* getUserInput() {
	int phy_size = 2, log_size = 1;
	char ch;
	char* str = NULL;

	if (str != NULL)
		free(str);

	str = (char*)malloc(2 * sizeof(char));
	checkMemoryAllocation(str);

	str[0] = getchar();
	while (str[log_size - 1] != '\n') {
		if (phy_size == log_size)
		{
			phy_size *= 2;
			str = realloc(str, phy_size);
			checkMemoryAllocation(str);
		}
		str[log_size] = getchar();
		log_size++;
	}

	str[log_size - 1] = '\0';
	if (phy_size < log_size)
	{
		str = realloc(str, log_size);
		checkMemoryAllocation(str);
	}

	return str;
}


/*this function recives a row and a col. the function checks if position is legal. return the result of the check*/
bool isLegalPos(char row, char col)
{
	if (!isLegalColChar(col))
		return false;
	if (!isLegalRowChar(row))
		return false;
	return true;
}

/*this function checks if a file was succesfully open. returns true if it did,
if it didnt the function prints an error message and returns false.*/
bool checkFileOpen(FILE* f)
{
	if (f == NULL)
	{
		return false;
	}
	else
		return true;
}
