#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


// display board
void dump(uint_fast32_t board)
{
	putchar('\n');

	for(int_fast8_t y=0; y<3; y++)
	{
		for(int_fast8_t x=0; x<3; x++)
		{
			int cell = board & 3;
			printf(" %02x", cell);
			board = board >> 2;
		}

		putchar('\n');
	}

	putchar('\n');
}


// check of game over state
bool final(uint_fast32_t board)
{
	uint_fast32_t a, b, c;

	// check rows
	for(int_fast8_t i=0; i<3; i++)
	{
		a = 3 & (board >> (6 * i));
		b = 3 & (board >> (6 * i + 2));
		c = 3 & (board >> (6 * i + 4));
		if((a & b & c) == 0)continue;
		if((a | b | c) == 3)continue;
		return true;
	}

	// check cols
	for(int_fast8_t i=0; i<3; i++)
	{
		a = 3 & (board >> (2 * i));
		b = 3 & (board >> (2 * i + 6));
		c = 3 & (board >> (2 * i + 12));
		if((a & b & c) == 0)continue;
		if((a | b | c) == 3)continue;
		return true;
	}

	// check diagonals
	for(int_fast8_t i=0; i<2; i++)
	{
		a = 3 & (board >> (4 * i));
		b = 3 & (board >> 8);
		c = 3 & (board >> (16 - 4 * i));
		if((a & b & c) == 0)continue;
		if((a | b | c) == 3)continue;
		return true;
	}

	// default
	return false;
}


// score game state using negmax
int_fast8_t score(uint_fast32_t board, uint_fast8_t player)
{
	// check if previous move has ended the game
	if(final(board))
	{
		return -30;
	}

	int_fast8_t val = -60;

	for(int_fast8_t i=0; i<9; i++)
	{
		// check if cell is empty
		uint_fast32_t cell = 3 & (board >> 2*i);
		if(cell != 0)continue;

		// recusive call
		uint_fast32_t new_board = board | (player << 2*i);
		int_fast8_t sub_score = -score(new_board, player ^ 3);

		// prefere shortest route
		sub_score -= sub_score > 0 ? 1 : -1;

		// calculate val
		val = sub_score > val ? sub_score : val;
	}

	return val;
}


// generate board from sequence of moves
uint_fast32_t walk(char *seq)
{
	uint_fast32_t board = 0;
	uint_fast32_t player = 1;

	int_fast8_t len = strlen(seq);
	for(int_fast8_t i=0; i<len; i++)
	{
		uint_fast32_t off = *seq - '0';

		// catch invalid character
		if(off > 9)
		{
			fprintf(stderr, "ERROR: invalid sequence\n");
			exit(-1);
		}

		// ignore "0" steps
		if(off != 0)
		{
			board |= player << (2 * off - 2);
		}

		player ^= 0x3;
		seq++;
	}

	return board;
}


int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		fprintf(stderr, "ERROR: no sequence found\n");
		exit(-1);
	}

	uint_fast32_t board = walk(argv[1]);
	uint_fast8_t player = 1 + (strlen(argv[1]) & 1);

	printf("\nnext: %d\n", player);

	dump(board);

	if(final(board))puts("game over");

	for(int_fast8_t i=0; i<9; i++)
	{
		// check if cell is empty
		uint_fast32_t cell = 3 & (board >> 2*i);
		if(cell != 0)continue;

		uint_fast32_t new_board = board | (player << 2*i);
		int res = -score(new_board, player);

		printf(">>> %d: %d\n", i+1, res);
		//dump(new_board);
	}

	putchar('\n');

	return 0;
}