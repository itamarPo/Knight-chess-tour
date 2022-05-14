#define PATH_TREE_GUARD
#include <stdbool.h>

#ifndef UTILS_GUARD
	#include "utils.h"
#endif // UTILS_GUARD
#ifndef CELL_LIST_GUARD
	#include "cell_list.h"
#endif // CELL_LIST_GUARD

#define BOARD_LENGTH 5

typedef struct _treeNodeListCell TreeNodeListCell;
typedef char chessPos[2];

typedef struct _treeNode {
	chessPos position;
	TreeNodeListCell *next_possible_positions;
} TreeNode;

typedef struct _treeNodeListCell {
	TreeNode *node;
	struct _treeNodeListCell *next;
} TreeNodeListCell;

typedef struct _pathTree {
	TreeNode *root;
} PathTree;

TreeNode *createNewTreeNode(chessPos position);
PathTree findAllPossibleKnightPaths(chessPos *startingPosition);
TreeNodeListCell *createNewTreeNodeListCell(chessPos position);
chessPosList *findKnightPathCoveringAllBoard(PathTree *path_tree);
chessPosList *findKnightPathCoveringAllBoardRecu(TreeNode *root, bool ***route);
TreeNode *findAllPossibleKnightPathsRecu(bool ***route, chessPosArray ***all_valid_positions, chessPos position);
bool isRouteCoveringAllBoard(bool ***route);
void freeRouteArray(bool **ptr);
void freePathTree(PathTree* tr);
void freePathTreeRecu(TreeNode* root);
