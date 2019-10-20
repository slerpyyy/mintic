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
			board >>= 2;
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
	uint_fast32_t sh_board = board;
	uint_fast32_t sh_player = player;
	uint_fast8_t new_player = player ^ 3;

	for(int_fast8_t i=0; i<9; i++)
	{
		// check if cell is empty
		if((sh_board & 3) == 0)
		{
			// recusive call
			int_fast8_t sub_score = -score(board | sh_player, new_player);

			// calculate val
			sub_score -= sub_score > 0 ? 1 : -1;
			if(sub_score > val)val = sub_score;
		}

		// update vars
		sh_player <<= 2;
		sh_board >>= 2;
	}

	return val;
}


// generate board from sequence of moves
uint_fast32_t walk(char *seq)
{
	uint_fast32_t board = 0;
	uint_fast32_t player = 1;

	char *sh_seq = seq;
	size_t len = strlen(seq);

	if(len > 64)
	{
		fprintf(stderr, "ERROR: Se- Senpai! The stwing it too big! UwU\n");
		exit(-1);
	}

	int_fast8_t iter = len;
	for(int_fast8_t i=0; i<iter; i++)
	{
		uint_fast32_t move = *sh_seq - '0';

		// catch invalid character
		if(move > 9)
		{
			fprintf(stderr, "ERROR: invalid sequence\n");
			goto SEQ_ERROR;
		}

		// ignore "0" moves
		if(move != 0)
		{
			uint_fast32_t off = 2 * move - 2;

			// check if cell is empty
			if((3 & (board >> off)) != 0)
			{
				fprintf(stderr, "ERROR: invalid move\n");
				goto SEQ_ERROR;
			}

			board |= player << off;
		}

		player ^= 0x3;
		sh_seq++;

		continue;

	SEQ_ERROR:
		fprintf(stderr, "seq: %s\n     ", seq);
		while(i --> 0)fputc(' ', stderr);
		fprintf(stderr, "^\n");
		exit(-1);
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

	// generate board form sequence
	uint_fast32_t board = walk(argv[1]);
	uint_fast8_t player = 1 + (strlen(argv[1]) & 1);

	// exit if game is aleady in final state
	if(final(board))
	{
		puts("Game over");
		exit(0);
	}

	// setup vars for loop
	int_fast8_t highscore = -100;
	int_fast8_t *score_board = (int_fast8_t *)malloc(9 * sizeof(int_fast8_t));

	// vars for better performance
	uint_fast32_t sh_board = board;
	uint_fast32_t sh_player = player;

	// loops through all possible moves
	for(int_fast8_t i=0; i<9; i++)
	{
		int_fast8_t curr_score = -100;

		// check if cell is empty
		uint_fast32_t cell = 3 & sh_board;
		if(cell == 0)
		{
			// calculate score
			curr_score = -score(board | sh_player, player ^ 3);

			// track highscore
			if(curr_score > highscore)highscore = curr_score;
		}

		// update vars
		score_board[i] = curr_score;
		sh_board >>= 2;
		sh_player <<= 2;
	}

	// setup format string
	const char *full_format = " %d";
	const char *format = full_format + 1;

	// print moves with highest score
	for(int_fast8_t i=0; i<9; i++)
	{
		if(score_board[i] < highscore)continue;

		printf(format, i + 1);
		format = full_format;
	}

	putchar('\n');
	free(score_board);

	return 0;
}
