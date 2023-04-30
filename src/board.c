/*
 * board.c - source code for board module
 *
 * See header file for function info
 *
 *
 * Sam Barton - April 2023
 */

#include "board.h"
#include <time.h>

typedef struct board {
  int r,c;
  int minesLeft;
  char** visible;
  char** hidden;
} board_t;

/*         board_create        */
board_t*
board_new (const int r, const int c, const int numMines) {
  board_t* b = malloc(sizeof(board_t));
  b->r=r;
  b->c=c;
  b->minesLeft=numMines;
  b->visible = malloc(r*sizeof(char*));
  b->hidden = malloc(r*sizeof(char*));
  for (int i = 0; i < r; i++) {
    b->visible[i] = malloc(c*sizeof(char));
    b->hidden[i] = malloc(c*sizeof(char));
    b->visible[i][c] = '\0';
    b->hidden[i][c] = '\0';
  }

  // fill with a's
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      b->visible[i][j] = 'a';
      b->hidden[i][j] = 'a';
    }
  }
  /* mine distribution algo */

  return b;
}

/*       board_click      */
void board_click (board_t* board, const int r, const int c) {
  // TODO
  // click action:
}


/*     board_flag      */
void board_flag (board_t* board, const int r, const int c) {
  // TODO
  // flag action
}

/*    board_print     */
void board_print (board_t* board) {
  if (board == NULL) {
    printf("invalid board");
  }
  int rows = board->r;
  int cols = board->c;
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      if (board->visible[r][c] == '\0') {
        printf("[0]\t");
      } else {
        printf("[%c]\t",board->visible[r][c]);
      }
    }
    printf("\n");
  }
}

/*       board_delete      */
void 
board_delete (board_t* board) {
  if (board == NULL) {
    fprintf(stderr, "invalid delete");
  }
  for(int i = 0; i < board->r; i++) {
    if(board->visible[i]!=NULL) free(board->visible[i]);
    if(board->hidden[i]!=NULL) free(board->hidden[i]);
  }
  free(board->visible);
  free(board->hidden);
  free(board);
}
