/*
 * header file for board.c
 *
 * Sam Barton - April 2023
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// board typedef
typedef struct board 
{
  int r,c;
  int minesLeft,squaresLeft;
  char** visible;
  char** hidden;
  bool empty;
} board_t;

enum DIFFICULTY{BEGINNER=1, INTERMEDIATE=2, EXPERT=3};

/*
 * board_new
 *
 * create a new empty board 
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
 *  on first click: generates mines for the empty board
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
 * board_auto
 *
 * game is in auto mode...flags square if hidden, clicks around if uncovered
 * 
 * we return:
 *  nothing, error message on bad parameters
 * we guarantee:
 *  board will update with action
 * 
 * caller is responsible for:
 *  passing valid params
 */
void board_auto(board_t* board, const int r, const int c);

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
 * board_to_string
 * formats board into a string for ncurses
 *
 * we return:
 *  nothing
 * 
 */
void board_to_string(board_t* board, char* string);
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
