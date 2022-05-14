#define CELL_LIST_GUARD

#include <stdio.h>
#include <stdlib.h>

#ifndef UTILS_GUARD
	#include "utils.h"
#endif // UTILS_GUARD

typedef char chessPos[2];
typedef unsigned char BYTE;

typedef struct _chessPosCell {
	chessPos position;
	struct _chessPosCell *next;
} chessPosCell;

typedef struct _chessPosList {
	chessPosCell *head;
	chessPosCell *end;
} chessPosList;


void removeCellsDuplication(chessPosList *lst);
chessPosList *makeEmptyList();
chessPosCell *createNewNode(chessPos position, chessPosCell* next);
void printList(chessPosList* lst);
void freeChessPosList(chessPosList** lst);
bool isListEmpty(chessPosList* lst);
void insertToTheEndList(chessPosList* lst, chessPos position);
void insertToTheHeadList(chessPosList* lst, chessPos position);
void removeCell(chessPosList *lst, chessPosCell* cell, chessPosCell* prev);

