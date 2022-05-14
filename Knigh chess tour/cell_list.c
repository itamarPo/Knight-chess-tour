#include "cell_list.h"

chessPosList *makeEmptyList() {
	chessPosList *lst = (chessPosList *)malloc(sizeof(chessPosList));
	checkMemoryAllocation(lst);

	chessPos pos = { 'X', 'X' }; // dummy head
	lst->end = lst->head = createNewNode(pos, NULL);
	return lst;
}

chessPosCell * createNewNode(chessPos position, chessPosCell * next) {
	chessPosCell * node = (chessPosCell *)malloc(sizeof(chessPosCell));
	checkMemoryAllocation(node);
	node->position[0] = position[0];
	node->position[1] = position[1];
	node->next = next;

	return node;
}

void insertToTheEndList(chessPosList* lst, chessPos position) {
	chessPosCell *new_node = createNewNode(position, NULL);
	if (lst->head->next == NULL)
		lst->head->next = new_node;
	lst->end = lst->end->next = new_node;
}

void insertToTheHeadList(chessPosList* lst, chessPos position) {
	chessPosCell *new_node = createNewNode(position, lst->head->next);
	if (lst->head == lst->end)
		lst->end = new_node;
	lst->head->next = new_node;
}

void removeCell(chessPosList *lst, chessPosCell* cell, chessPosCell* prev) {
	if (cell->next == NULL)
	{
		lst->end = prev;
		prev->next = NULL;
	}
	else
		prev->next = cell->next;
}

bool isListEmpty(chessPosList *lst) {
	if (lst == NULL || lst->head == NULL || lst == NULL || lst->head->next == NULL)
		return true;
	return false;
}

void printList(chessPosList *lst) {
	chessPosCell *curr = lst->head->next;
	printf("chessPosList: ");
	while (curr != NULL)
	{
		printf("(%c,%c) ", curr->position[0], curr->position[1]);
		curr = curr->next;
	}
	printf("\n");
}

void freeChessPosList(chessPosList **lst) {
	chessPosCell *tmp;
	chessPosCell *curr = (*lst)->head->next;
	printf("[!] freeing the list of positions.\n");

	while (curr != NULL)
	{
		tmp = curr->next;
		free(curr);
		curr = tmp;
	}
	*lst = NULL;
}


/* Gets list of cells and remove dups*/
void removeCellsDuplication(chessPosList *lst) {
	int cells_counter[BOARD_LENGTH][BOARD_LENGTH];
	chessPosCell *prev = lst->head->next;
	chessPosCell *curr = lst->head->next;
	while (curr != NULL)
	{
		if (cells_counter[charIndexToInt(curr->position[0])][charIndexToInt(curr->position[1])] == 1)
		{
			removeCell(lst, curr, prev);
			curr = prev->next;
		}
		else
		{
			cells_counter[charIndexToInt(curr->position[0])][charIndexToInt(curr->position[1])] = 1;
			prev = curr;
			curr = curr->next;
		}
	}

}