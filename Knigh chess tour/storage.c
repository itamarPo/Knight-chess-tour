#include "storage.h"

/*this function recives a file name and a list of positions.
the function creates a new file (named by the recived name),
and saves the positions in the list inside the file.*/
bool saveListToBinFile(char* file_name, chessPosList* pos_list)
{
	int bytes_len;
	unsigned short length;
	BYTE *bytes = NULL;
	FILE* f = fopen(file_name, "wb");
	if (!checkFileOpen(f))
		exit(2);
	length = (unsigned short)getListLength(pos_list);
	fwrite(&length, sizeof(unsigned short), 1, f);

	if (length != 0)
	{
		bytes = getConcatenatedBytes(pos_list, length, &bytes_len);
		writeBytesToFile(f, bytes, bytes_len);
		free(bytes);
	}
	
	fclose(f);

	return true;
}

/*this function recives a row and a column and converts them into a BYTE.
the function returns the new created BYTE.*/
BYTE resultOfByte(char row, char col)
{
	BYTE row_byte = charIndexToInt(row);
	BYTE col_byte = charIndexToInt(col);
	BYTE byte = 0x0;
	byte = byte | row_byte;
	byte = byte << 3;
	byte = byte | col_byte;
	byte = byte << 2;


	return byte;
}

/*this function recives a list and returns its length*/
int getListLength(chessPosList* pos_list)
{
	int counter = 0;
	chessPosCell* curr = pos_list->head->next;
	while (curr != NULL)
	{
		counter++;
		curr = curr->next;
	}
	return counter;
}

/*this function recives a list of positions and a length.
the function creates and returns a new array of bytes the size of length which contains the positions of the list.*/
BYTE* getBytesArray(chessPosList* pos_list, unsigned short length) {
	int i = 0;
	BYTE* bytes_arr = (BYTE*)malloc(length * sizeof(BYTE));
	checkMemoryAllocation(bytes_arr);
	chessPosCell* curr = pos_list->head->next;

	while (curr != NULL)
	{
		bytes_arr[i] = resultOfByte(curr->position[0], curr->position[1]);
		curr = curr->next;
		i++;
	}
	return bytes_arr;
}

/*this function recives a file and an array of bytes and its length.
the function writes the array of byte inside the file.*/
void writeBytesToFile(FILE *f, BYTE* bytes, int length) {
	int i;
	for (i = 0; i < length; i++)
		fwrite(&bytes[i], sizeof(BYTE), 1, f);
}

/*this function recives a file, a positions list and its length.
the function creates a new array of bytes which contains the positions of the lists,
then it reorganizes the bits inside the array.
after the reorganization the array of bytes will includes a 6 bits to any positions as required.
the function returns the new created array of BYTES.*/
BYTE *getConcatenatedBytes(chessPosList* pos_list, unsigned short length, int *bytes_len) {
	int  i = 0, index_b = 0, free_space = 0;
	BYTE* bytes_arr = getBytesArray(pos_list, length);
	BYTE* bytes = NULL;
	BYTE mask;
	int bytes_length = ceil(length * 6 / 8.0);

	if ((length * 6 % 8) == 0)
		bytes_length++;

	bytes = (BYTE*)malloc(bytes_length * sizeof(BYTE));
	checkMemoryAllocation(bytes);

	for (i = 0; i < length; i++)
	{
		free_space = (i % 4) * 2;
		if (free_space != 0)
		{
			mask = bytes_arr[i] >> 8 - free_space;
			bytes[index_b] = bytes[index_b] | mask;
			index_b++;
			bytes[index_b] = bytes_arr[i] << free_space;
		}
		else
			bytes[index_b] = bytes_arr[i];
	}

	free(bytes_arr);

	*bytes_len = index_b + 1;
	return bytes;
}


/*this function recives a name of the file.
the function opens the file (by the recived name) and then it creates a new list of positions from the file.
if the file wasn't found the function will return -1.
else, if the newly created list from the file has illegal moves (or positions) the function will returns 1.
if the function contains legal moves, then the function will display the created list and will remove any duplications.
Afterwards, the function will check if the list covers the whole board, if it does the function will return 2.
else the function will return 3.*/
int checkAndDisplayPathFromFile(char* file_name)
{
	FILE* f;
	int i;
	unsigned short num_of_moves;
	f = fopen(file_name, "rb");
	if (!checkFileOpen(f))
		return -1;
	fread(&num_of_moves, sizeof(short), 1, f);
	chessPosList* pos_list = getPosFromFile(f, num_of_moves);
	fclose(f);

	if (pos_list == NULL)
		return 1;
	if (num_of_moves != 0)
		if (!checkLegalPath(pos_list))
		{
			printList(pos_list);
			freeChessPosList(pos_list);
			return 1;
		}
	display(pos_list);

	if (getListLength(pos_list) == BOARD_LENGTH * BOARD_LENGTH)
		return 2;
	else
		return 3;

}

/*this function recives a file and size of positions to read.
the function reads the bits from the file and creates a list of positions.
then the function close the file and returns the new list. */
chessPosList* getPosFromFile(FILE* f, unsigned short size)
{
	unsigned short i;
	int bits_left, bits_right;
	BYTE byte, mask;
	chessPos pos;
	chessPosList* list_of_pos = makeEmptyList();
	if (size == 0)
		return list_of_pos;
	fread(&byte, sizeof(BYTE), 1, f);


	for (i = 0; i < size; i++)
	{
		bits_right = (i % 4) * 2;
		bits_left = bits_right + 2;

		if (i % 4 == 0)
		{
			if (ByteToPosition(byte, &pos) == -1)
				return NULL;
			insertToTheEndList(list_of_pos, pos);
			mask = byte << (8 - bits_left);
		}
		else
		{
			fread(&byte, sizeof(BYTE), 1, f);
			if (feof(f))
				return NULL;
			mask = getMask(byte, mask, bits_right);
			if (ByteToPosition(mask, &pos) == -1)
				return NULL;
			insertToTheEndList(list_of_pos, pos);
			mask = byte << (8 - bits_left);
		}
	}
	return list_of_pos;
}


/*this function recives a byte and a output varient pos.
the function updates the pos (by the recived byte), and check if the recived position is legal
if it isnt legal the function returns -1, else it returns 1.*/
int ByteToPosition(BYTE byte, chessPos *pos)
{
	BYTE location;
	location = byte << 3;
	location = location >> 5;
	location += '1';
	(*pos)[1] = location;

	location = (byte >> 5);
	location += 'A';
	(*pos)[0] = location;

	if (!isLegalPos((*pos)[0], (*pos)[1]))
		return -1;
	else
		return 1;
}



/*this function recives lists of positions, the function checks if it has a legal moves and positions for a knight.
the function returns the result of the check.*/
bool checkLegalPath(chessPosList* list_of_pos)
{
	chessPosCell* curr = list_of_pos->head->next;
	chessPosArray*** valid_moves = validKnightMoves();
	int num_of_moves, row, col;
	if (list_of_pos->head == list_of_pos->end)
	{
		if (isLegalPos(curr->position[0], curr->position[1]))
			return true;
		else
			return false;
	}

	while (curr != NULL && curr->next != NULL)
	{
		if (isLegalPos(curr->position[0], curr->position[1]))
		{
			row = charIndexToInt(curr->position[0]);
			col = charIndexToInt(curr->position[1]);
			num_of_moves = valid_moves[row][col]->size;
			if (!isNextMoveLegal(curr->next, valid_moves[row][col], num_of_moves))
				return false;
			curr = curr->next;
		}
		else
			return false;
	}
	return true;
}

/*this function recives a position cell and an array of positions and its size.
the function checks if the position inside the cell is present in the array of positions.
the function returns the result of the check*/
bool isNextMoveLegal(chessPosCell* recived_pos, chessPosArray* arr_of_moves, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		if (recived_pos->position[0] == arr_of_moves->positions[i][0])
			if (recived_pos->position[1] == arr_of_moves->positions[i][1])
				return true;
	}
	return false;
}

/*this function recives a byte, a previous mask and integer which determends how many bits to the right we move.
the function makes a new mask, (updating the previous mask) and returns it.*/
BYTE getMask(BYTE byte, BYTE previous_mask, int bits_right)
{
	BYTE mask_manipulate = byte;
	mask_manipulate = mask_manipulate >> bits_right;
	previous_mask = previous_mask | mask_manipulate;
	return previous_mask;
}