/*
 * minesweeper - main program for minesweeper game
 * Sam Barton - Apr 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include "board.h"

static void parseArgs(const int argc, char* argv[], int* difficulty);
static int gameLoop(board_t* board);

int
main (int argc, char* argv[]) {
  int difficulty = 4;
  parseArgs(argc, argv, &difficulty);
  board_t* board = NULL;
  if (difficulty == BEGINNER) {
    board = board_new(9, 9, 10);
  } else if (difficulty == INTERMEDIATE) {
    board = board_new(16, 16, 40);
  } else if (difficulty == EXPERT) {
    board = board_new(16, 30, 99);
  }
  board_print(board);
  gameLoop(board);
  board_delete(board);
  return 0;
}

static void
parseArgs (const int argc, char* argv[], int* difficulty) {
  if (argc != 2) {
    printf("usage: %s difficulty=1,2,3\n",argv[0]);
    exit(1);
  }
  *difficulty = atoi(argv[1]);
  if (*difficulty == 0) {
    printf("usage: %s difficulty=1,2,3\n",argv[0]);
    exit(1);
  } 
}

static int
gameLoop(board_t* board) {
  while (1) {
    if(boardWon(board) == 1) {
      printf("You Won!\n");
      return 0;
    }
    printf("\nAction? ");
    int c = -1;
    char r='\0', ch = '\0';
    if(scanf(" %c",&ch)!=1) {printf("error\n");continue;};
    if (ch == 'f') {
      scanf(" %c%d",&r,&c);
      board_flag(board, r-97, c);
    } else if (ch == 'c') {
      scanf(" %c%d",&r,&c);
      board_click(board, r-97, c);
    } else if (ch == 'q') {
      board_delete(board);
      exit(0);
    }
  }
  return 1;
}
