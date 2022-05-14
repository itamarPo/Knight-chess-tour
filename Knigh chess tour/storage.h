#define _CRT_SECURE_NO_WARNINGS
#define STORAGE_GUARD

#include <stdlib.h>
#include <stdbool.h>


#ifndef CELL_LIST_GUARD
	#include "cell_list.h"
#endif // CELL_LIST_GUARD

typedef unsigned char BYTE;


bool saveListToBinFile(char* file_name, chessPosList* pos_list);
BYTE resultOfByte(char row, char col);
int getListLength(chessPosList* pos_list);
BYTE *getConcatenatedBytes(chessPosList* pos_list, unsigned short length, int *bytes_len);
void writeBytesToFile(FILE *f, BYTE* bytes, int length);
int checkAndDisplayPathFromFile(char* file_name);
chessPosList* getPosFromFile(FILE* f, unsigned short size);
int ByteToPosition(BYTE bit, chessPos *pos);
bool checkLegalPath(chessPosList* list_of_pos);
bool isNextMoveLegal(chessPosCell* recived_pos, chessPosArray* arr_of_moves, int size);
BYTE getMask(BYTE byte, BYTE previous_mask, int bits_right);
