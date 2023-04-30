/*
 * minesweeper - main program for minesweeper game
 * Sam Barton - Apr 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include "board.h"

int
main (int argc, char** argv) {
  board_t* board = board_new(3, 3, 6);
  board_print(board);
  board_delete(board);
  return 0;
}
