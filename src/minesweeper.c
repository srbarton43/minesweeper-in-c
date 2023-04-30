/*
 * minesweeper - main program for minesweeper game
 * Sam Barton - Apr 2023
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct board {
  int r,c;
  int minesLeft;
  int** brd;
} board_t;

static board_t* createBoard(int r, int c, int numMines);
static void printBoard(board_t* board);

int
main (int argc, char** argv) {
  board_t* board = createBoard(4, 5, 2);
  printBoard(board);
  return 0;
}

static board_t*
createBoard (int r, int c, int numMines) {
  board_t* b= malloc(sizeof(board_t));
  b->r = r;
  b->c = c;
  b->minesLeft = numMines;
  b->brd = malloc(r*sizeof(int));
  for (int i=0; i<r; i++) {
    b->brd[i] = malloc(c*sizeof(int));
  }
  return b;
}

static void
printBoard (board_t* board) {
  for(int r = 0; r < board->r; r++) {
    for(int c = 0; c < board->c; c++) {
      printf("[] ");
    }
    printf("\n");
  }
}
