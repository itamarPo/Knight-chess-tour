#include "path_tree.h"

// free route array
void freeRouteArray(bool **ptr) {
	int i;

	for (i = 0; i < BOARD_LENGTH; i++)
		free(ptr[i]);
	free(ptr);
}

bool **getRouteArray() {
	int i,j;
	bool **route = (bool **)malloc(BOARD_LENGTH * sizeof(bool*));
	checkMemoryAllocation(route);

	// setup route array, set all to false.
	for (i = 0; i < BOARD_LENGTH; i++)
	{
		route[i] = (bool *)malloc(BOARD_LENGTH * sizeof(bool));
		checkMemoryAllocation(route[i]);

		for (j = 0; j < BOARD_LENGTH; j++)
			route[i][j] = false;
	}
	
	return route;
}

// search for a route the covers all board and return it in a list
chessPosList *findKnightPathCoveringAllBoard(PathTree *path_tree) {
	int i, j;
	chessPosList *lst;
	bool **route = getRouteArray();
	
	// setup route array, set all to false.
	for (i = 0; i < BOARD_LENGTH; i++)
	{
		route[i] = (bool *)malloc(BOARD_LENGTH * sizeof(bool));
		checkMemoryAllocation(route[i]);

		for (j = 0; j < BOARD_LENGTH; j++)
			route[i][j] = false;
	}

	lst = findKnightPathCoveringAllBoardRecu(path_tree->root, &route);

	// free route array
	freeRouteArray(route);

	return lst;
}

// search for a route the covers all board and return it in a list recursion function
chessPosList * findKnightPathCoveringAllBoardRecu(TreeNode *root, bool ***route) {
	chessPosList *lst = NULL;
	TreeNodeListCell *curr;
	bool *route_block = &((*route)[charIndexToInt(root->position[0])][charIndexToInt(root->position[1])]);

	// if cell already has been traveled before, we need to end this sub-recursion
	if (*route_block)
	{
		// if all cells are marked in the route
		if (isRouteCoveringAllBoard(route))
			return makeEmptyList();

		return NULL;
	}

	*route_block = true;

	curr = root->next_possible_positions; 
	while(curr != NULL) {
		lst = findKnightPathCoveringAllBoardRecu(curr->node, route);
		if (lst != NULL)
		{
			insertToTheHeadList(lst, root->position);
			return lst;
		}
			
		curr = curr->next;
	}

	*route_block = false;

	return NULL;
}

// define PathTree contain all the possible knight path exists of a given starting position
PathTree findAllPossibleKnightPaths(chessPos *starting_position) {
	int i, j;
	PathTree tr;
	chessPosArray ***valid_positions = validKnightMoves();
	TreeNode *node = createNewTreeNode(starting_position);
	bool **route = getRouteArray();

	tr.root = NULL;
	tr.root = findAllPossibleKnightPathsRecu(&route, valid_positions, *starting_position);
	
	// free route array
	freeRouteArray(route);

	return tr;
}



// gets root of a tree and position, the root using to check if one of the valid positions is already walked before TBC....
TreeNode * findAllPossibleKnightPathsRecu(bool ***route, chessPosArray ***all_valid_positions, chessPos position) {
	int i, row_index, col_index;
	TreeNodeListCell *prev = NULL, *curr;
	TreeNode *node = createNewTreeNode(position);
	chessPosArray *valid_positions = NULL;

	row_index = charIndexToInt(position[0]);
	col_index = charIndexToInt(position[1]);
	if ((*route)[row_index][col_index] == true)
		return node;

	(*route)[row_index][col_index] = true;
	valid_positions = all_valid_positions[row_index][col_index];

	for (i = 0; i < valid_positions->size; i++)
	{
		curr = createNewTreeNodeListCell(valid_positions->positions[i]);

		//if the first node
		if (prev == NULL)
			node->next_possible_positions = curr;
		else
			prev->next = curr;
		curr->node = findAllPossibleKnightPathsRecu(route, all_valid_positions, valid_positions->positions[i]);
		prev = curr;
	}

	(*route)[row_index][col_index] = false;
	return node;
}

TreeNode* createNewTreeNode(chessPos position)
{
	TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
	checkMemoryAllocation(node);

	node->position[0] = position[0];
	node->position[1] = position[1];
	node->next_possible_positions = NULL;

	return node;
}

void freePathTree(PathTree* tr) {
	printf("[!] freeing path tree.\n");
	freePathTreeRecu(tr->root);
	tr->root = NULL;
}

void freePathTreeRecu(TreeNode* root) {
	int i;
	TreeNodeListCell *curr, *tmp;
	if (root == NULL)
		return;
	
	curr = root->next_possible_positions;
	while (curr != NULL) {
		tmp = curr->next;
		freePathTreeRecu(curr->node);
		free(curr);
		curr = tmp;
	}

	free(root);
}

TreeNodeListCell *createNewTreeNodeListCell(chessPos position) {
	TreeNodeListCell *cell = (TreeNodeListCell *)malloc(sizeof(TreeNodeListCell));
	checkMemoryAllocation(cell);

	cell->node = NULL;
	cell->next = NULL;

	return cell;
}


// check if the route is all marked as true
bool isRouteCoveringAllBoard(bool ***route) {
	int i, j, c = 0;
	for (i = 0; i < BOARD_LENGTH; i++)
		for (j = 0; j < BOARD_LENGTH; j++)
		{
			if (!(*route)[i][j])
				return false;
		}
	return true;
}