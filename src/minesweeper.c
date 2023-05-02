/*
 * minesweeper - main program for minesweeper game
 * Sam Barton - Apr 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include "board.h"

static void parseArgs(const int argc, char* argv[], int* rows, int* cols, int *numMines);
static int gameLoop(board_t* board);

int
main (int argc, char* argv[]) {
  int rows = 0, cols = 0, numMines = 0;
  parseArgs(argc, argv, &rows, &cols, &numMines);
  board_t* board = NULL;
  if (numMines != 0) {
    board = board_new(rows, cols, numMines);
  } else {
    board =  board_new(cols, rows, .15*cols*rows);
  }
  board_print(board);
  gameLoop(board);
  board_delete(board);
  return 0;
}

static void
parseArgs (const int argc, char* argv[], int* rows, int* cols, int* numMines) {
  if (argc != 3 && argc != 4) {
    printf("usage: nRows nCols [nMines]\n");
    exit(1);
  }
  *rows = atoi(argv[1]);
  *cols = atoi(argv[2]);
  if (argc == 4) {
    *numMines = atoi(argv[3]);
  }
  if (*rows == 0 || *cols == 0 || numMines == 0) {
    printf("usage: nRows nCols [nMines]\n");
    exit(2);
  }
}

static int
gameLoop(board_t* board) {
  while (1) {
    if(boardWon(board) == 1) {
      printf("You Won!\n");
      return 0;
    }
    printf("Action? ");
    fflush(stdout);
    int r = -1, c = -1;
    char ch = '\0';
    scanf("%c %d %d", &ch, &r, &c);
    printf("\n");
    if (ch == 'f') {
      board_flag(board, r, c);
    } else if (ch == 'c') {
      board_click(board, r, c);
    } else if (ch == 'q') {
      board_delete(board);
      exit(0);
    }
  }
  return 1;
}
