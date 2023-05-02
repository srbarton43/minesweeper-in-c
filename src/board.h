/*
 * header file for board.c
 *
 * Sam Barton - April 2023
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdlib.h>
#include <stdio.h>

// board typedef

typedef struct board board_t;

/*
 * board_new
 *
 * create a new board with random mines
 *
 * we return:
 *  pointer to new board type
 *  or NULL on error
 *
 * caller is responible for:
 *  passing valid parameters
 *  calling board_delete
 *
 */
board_t* board_new(const int r, const int c, const int numMines);


/*
 * board_click
 *
 * user clicks a square - determine the action
 *
 * we return:
 *  nothing, error message on bad parameters
 * we guarantee:
 *  baord will update according to action
 *
 * caller is resposible for:
 *  passing valid parameters
 *
 */
void board_click(board_t* board, const int r, const int c);


/*
 * board_flag
 *
 * user flags a square
 *
 * we return: 
 *  nothing, error message on bad parameters
 * we guarantee:
 *  board will update with the flagged tile
 *
 * caller is responsible for:
 *  passing valid parameters
 */
void board_flag(board_t* board, const int r, const int c);

/*
 * boardWon
 * 
 * Checks if user has won
 * 
 * Returns:
 *  1 if won
 *  0 if not
 */
int boardWon(board_t* board);
/*
 * board_print
 *
 * print visible board to stdout
 *
 * we return:
 *  nothing
 * 
 * caller is responsible for:
 *  passing valid parameters
 *
 */
void board_print(board_t* board);


/* 
 * board_delete
 *
 * delete the board and free alloc'ed memory
 *
 * we return:
 *  nothing, error message on error
 * we guaranteeL
 *  all allocated memory will be freed for the board
 *
 * caller is responsible for:
 *  passing valid board_t*
 *
 */
void board_delete(board_t* board);

#endif
