/*
 * minesweeper - main program for minesweeper game
 * Sam Barton - Apr 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include "board.h"

int
main (int argc, char** argv) {
  board_t* board = board_new(4, 5, 2);
  board_print(board);
  board_delete(board);
  return 0;
}
