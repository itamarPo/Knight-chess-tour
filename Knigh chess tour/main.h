#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>

#ifndef UTILS_GUARD
	#include "utils.h"
#endif // UTILS_GUARD

#ifndef CELL_LIST_GUARD
#include "cell_list.h"
#endif // CELL_LIST_GUARD

#ifndef STORAGE_GUARD
#include "storage.h"
#endif // STORAGE_GUARD

#ifndef PATH_TREE_GUARD
#include "path_tree.h"
#endif // PATH_TREE_GUARD

void display(chessPosList *lst);
void displayMenu(chessPos *starting_position);
void printBoard(int **board);
void printBoardDividerLine(int length);
void printBoardDividerLine(int length);
void getFromUserAStartingPosition(chessPos *starting_position);
void displayMenu(chessPos *starting_position);
bool menuOptionFindAllPossibleKnightPaths(chessPos *starting_position, PathTree *tr);
bool menuOptionFindKnightPathCoveringAllBoard(chessPos *starting_position, PathTree *tr, chessPosList **lst);
void menuOptionSaveListToBinFile(chessPos *starting_position, PathTree *tr, chessPosList **lst);
void menuOptionCheckAndDisplayPathFromFile();
void freeTreeAndListIfNeeded(PathTree *tr, chessPosList **lst);
void freeMemory(chessPos *starting_position, PathTree *tr, chessPosList *lst);
void main();