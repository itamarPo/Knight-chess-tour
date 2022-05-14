#define _CRT_SECURE_NO_WARNINGS
#define UTILS_GUARD
#define BOARD_LENGTH 5

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


typedef char chessPos[2];

typedef struct _chessPosArray {
	unsigned int size;
	chessPos* positions;
} chessPosArray;

chessPosArray*** validKnightMoves();
chessPosArray* LegitMoves(int row, int col);
chessPos* Moves(int row, int col, int* size);
bool isLegalIndex(int index);
char IntToCharCol(int num);
char IntToCharRow(int num);
int charIndexToInt(char c);
void checkMemoryAllocation(void *ptr);
bool checkFileOpen(FILE* f);
void printChessPosition(chessPos position);
bool isLegalRowChar(char row);
bool isLegalColChar(char col);
bool isLegalPos(char row, char col);
char *getUserInput();
void freeValidKnightMoves(chessPosArray*** valid_moves);