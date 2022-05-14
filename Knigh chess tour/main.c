#include "main.h"

/* Prints a divider line */
void printBoardDividerLine(int length) {
	int i,j;
	for (i = 0; i < length; i++)
	{
		printf("+");
		for (j = 0; j < 4; j++)
		{
			printf("-");
		}
		
	}
	printf("+\n");
}

/* Prints board, gets two deimension int array which holds each cell content */
void printBoard(int **board) {
	int i, j;
	printBoardDividerLine(BOARD_LENGTH + 1);
	for (i = 0; i < BOARD_LENGTH + 1; i++)
	{
		printf("|%3d ", i);
	}
	printf("|\n");
	printBoardDividerLine(BOARD_LENGTH + 1);
	for (i = 0; i < BOARD_LENGTH; i++)
	{
		printf("|%3c ", i + 'A');
		for (j = 0; j < BOARD_LENGTH; j++)
		{
			if (board[i][j] == 0)
				printf("|    ");
			else
				printf("|%3d ", board[i][j]);
		}
		printf("|\n");
		printBoardDividerLine(BOARD_LENGTH + 1);
	}	
}

// get list of a knight path and prints it  in a board 
void display(chessPosList *lst) {
	int **board;
	int i, counter = 0;
	chessPosCell *curr = lst->head->next;
	removeCellsDuplication(lst);

	printList(lst);
	board = (int **)malloc(BOARD_LENGTH * sizeof(int *));
	checkMemoryAllocation(board);
	for (i = 0;i < BOARD_LENGTH; i++)
	{
		board[i] = (int *)calloc(BOARD_LENGTH, sizeof(int));
		checkMemoryAllocation(board[i]);
	}
	
	while (curr != NULL)
	{
		counter++;
		board[charIndexToInt(curr->position[0])][charIndexToInt(curr->position[1])] = counter;
		curr = curr->next;
	}

	printBoard(board);

	// free board array
	for (i = 0;i < BOARD_LENGTH; i++)
		free(board[i]);

	free(board);
}

// get input from user to define the knight starting position
void getFromUserAStartingPosition(chessPos *starting_position) {
	char user_row, user_col;
	char *input;

	printf("Enter a knight starting position (Examples: A 1 , B 4) ->");
	input = getUserInput();
	if (strlen(input) != 3)
	{
		printf("Invalid Input, returned to menu");
		return;
	}
	if (input == NULL || strlen(input) != 3)
	{
		printf("[-] Invalid input format, sending you back to menu.\n");
		return;
	}
	sscanf(input, "%c %c", &user_row, &user_col);
	
	if (!isLegalRowChar(user_row))
	{
		printf("[-] Invalid row entered: %c\n", user_row);
		return;
	}
	if (!isLegalColChar(user_col))
	{
		printf("[-] Invalid col entered: %c\n", user_col);
		return;
	}

	(*starting_position)[0] = user_row;
	(*starting_position)[1] = user_col;
	printf("[+] New starting Position has been defined.\n");
}

// menu prints
void displayMenu(chessPos *starting_position) {
	
	printf("\nDefined starting position: ");
	if ((*starting_position)[0] != 0)
		printf("%c,%c\n", (*starting_position)[0], (*starting_position)[1]);
	else
		printf("NULL\n");
	printf("1. Enter a knight's starting position\n");
	printf("2. Create all possible knight paths\n");
	printf("3. Find a knight path covering all board\n");
	printf("4. Save knight path covering all board to file\n");
	printf("5. Load and display path from file\n");
	printf("6. Exit\n");
}

// check validation of calling find all possible knight path function and run if ok
bool menuOptionFindAllPossibleKnightPaths(chessPos *starting_position, PathTree *tr) {
	if ((*starting_position)[0] == 0)
	{
		printf("[-] You need to define a starting position first.\n");
		return false;
	}
	
	// check if path tree need to be recalculate
	if (tr->root == NULL || tr->root->position[0] != (*starting_position)[0] || tr->root->position[1] != (*starting_position)[1])
	{
		printf("Loading...\n");
		*tr = findAllPossibleKnightPaths(starting_position);
		printf("[+] Done!\n");
	}
	else
		printf("[!] Path tree already has been calculated for this position");
	
	return true;
}

// check validation of calling find path covering all board function and run if ok
bool menuOptionFindKnightPathCoveringAllBoard(chessPos *starting_position, PathTree *tr, chessPosList **lst) {
	if (tr->root == NULL)
	{
		printf("[-] Path tree is not defined yet.\n[!] Calling Auto path tree search.\n");
		if (!menuOptionFindAllPossibleKnightPaths(starting_position, tr))
			return false;
	}
	if (!isListEmpty(*lst))
		freeChessPosList(lst);
	*lst = findKnightPathCoveringAllBoard(tr);

	if (*lst == NULL)
	{
		printf("[-] No path covering all board has been found.\n");
		return false;
	}

	display(*lst);
	return true;
}

// before running SaveListToBinFile feature, get file name and check valdiation
void menuOptionSaveListToBinFile(chessPos *starting_position, PathTree *tr, chessPosList **lst) {
	char *file_name;
	bool file_saved = false;
	printf("Enter file name -> ");
	file_name = getUserInput();

	if (isListEmpty(*lst))
	{
		printf("[-] Chess position list is empty.\n[!] Auto Calling find knight path covering all board function.\n");
		if (menuOptionFindKnightPathCoveringAllBoard(starting_position, tr, lst))
			file_saved = saveListToBinFile(file_name, *lst);
	}
	else 
		file_saved = saveListToBinFile(file_name, *lst);

	if (file_saved)
		printf("[+] Position List data saved in %s\n", file_name);
	else
		printf("[-] saving file aborted.\n");

	free(file_name);
}

// before running CheckAndDisplayPathFromFile, get file name and check valdiation
void menuOptionCheckAndDisplayPathFromFile() {
	char *file_name;
	int returned_value;
	printf("Enter file name to load -> ");
	file_name = getUserInput();

	returned_value = checkAndDisplayPathFromFile(file_name);

	switch (returned_value) {
	case -1:
		printf("[-] File %s not found.\n", file_name);
		break;
	case 1:
		printf("[-] Invalid knight's path.\n");
		break;
	case 2:
		printf("[+] The knight's path is covering the whole board.\n");
		break;
	case 3:
		printf("[!]  The knight's path is not covering the whole board.\n");
		break;
	default:
		break;
	}

	free(file_name);
}

// when selecting new starting position, freeing path tree or chess ppositions list if needed 
void freeTreeAndListIfNeeded(PathTree *tr, chessPosList **lst) {
	if ((*tr).root != NULL)
		freePathTree(tr);
	if (!isListEmpty(*lst))
		freeChessPosList(lst);
}

void freeMemory(chessPos *starting_position, PathTree *tr, chessPosList *lst) {
	freeValidKnightMoves(validKnightMoves());
	free(starting_position);
	freeTreeAndListIfNeeded(tr, &lst);
}

void main() {
	int res, user_option;
	bool keep_loop = true;
	PathTree tr;
	char *input;
	chessPosList *lst = NULL;
	chessPos *starting_position = (chessPos *)malloc(sizeof(chessPos));
	checkMemoryAllocation(starting_position);

	tr.root = NULL;

	(*starting_position)[0] = (*starting_position)[1] = 0;
	printf("\nWelcome.\n");

	while (keep_loop) {
		displayMenu(starting_position);

		printf("Choose option from the menu -> ");
		input = getUserInput();
		if (strlen(input) != 1 || !(input[0] >= '1' && input[0] <= '6'))
		{
			printf("[-] Invalid Input!\n");
			continue;
		}
		sscanf(input, "%d", &user_option);
		system("cls");

		switch (user_option)
		{
		case 1:
			freeTreeAndListIfNeeded(&tr, &lst);
			getFromUserAStartingPosition(starting_position);
			break;
		case 2:
			menuOptionFindAllPossibleKnightPaths(starting_position, &tr);
			break;
		case 3:
			menuOptionFindKnightPathCoveringAllBoard(starting_position, &tr, &lst);
			break;
		case 4:
			menuOptionSaveListToBinFile(starting_position, &tr, &lst);
			break;
		case 5:
			menuOptionCheckAndDisplayPathFromFile();
			break;
		case 6:
			keep_loop = false;
			break;
		default:
			break;
		}
	}

	freeMemory(starting_position, &tr, lst);
	printf("\nBye bye.\n");
}