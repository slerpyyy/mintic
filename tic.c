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
	for(int_fast8_t i=0; i<12; i+=6)
	{
		a = 3 & (board >> i);
		b = 3 & (board >> (i + 2));
		c = 3 & (board >> (i + 4));
		if((a & b & c) == 0)continue;
		if((a | b | c) == 3)continue;
		return true;
	}

	// check cols
	for(int_fast8_t i=0; i<6; i+=2)
	{
		a = 3 & (board >> i);
		b = 3 & (board >> (i + 6));
		c = 3 & (board >> (i + 12));
		if((a & b & c) == 0)continue;
		if((a | b | c) == 3)continue;
		return true;
	}

	// check diagonals
	for(int_fast8_t i=0; i<8; i+=4)
	{
		a = 3 & (board >> i);
		b = 3 & (board >> 8);
		c = 3 & (board >> (16 - i));
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
	if(final(board))return -30;

	// default return val
	int_fast8_t val = -60;

	// setup vars to speed up loop
	uint_fast32_t shift_board = board;
	uint_fast32_t shift_player = player;
	uint_fast8_t new_player = player ^ 3;

	for(int_fast8_t i=0; i<9; i++)
	{
		// check if cell is empty
		if((shift_board & 3) == 0)
		{
			// recusive call
			int_fast8_t sub_score = -score(board | shift_player, new_player);

			// calculate val
			sub_score -= sub_score > 0 ? 1 : -1;
			val = sub_score > val ? sub_score : val;
		}

		// update vars
		shift_player = shift_player << 2;
		shift_board = shift_board >> 2;
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
		if(off != 0)board |= player << (2 * off - 2);

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

	if(final(board))
	{
		puts("Game over");
		exit(0);
	}


	int_fast8_t *score_board = (int_fast8_t *)malloc(9 * sizeof(int_fast8_t));
	int_fast8_t highscore = -100;

	for(int_fast8_t i=0; i<9; i++)
	{
		score_board[i] = -100;

		// check if cell is empty
		uint_fast32_t cell = 3 & (board >> 2*i);
		if(cell != 0)continue;

		// calculate score
		uint_fast32_t new_board = board | (player << 2*i);
		int_fast8_t res = (score_board[i] = -score(new_board, player ^ 3));

		// track highscore
		if(res > highscore)highscore = res;
	}

	for(int_fast8_t i=0; i<9; i++)
	{
		if(score_board[i] < highscore)continue;
		printf("%d ", i + 1);
	}

	putchar('\n');

	return 0;
}