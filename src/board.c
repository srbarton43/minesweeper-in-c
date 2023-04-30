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
board_new (const int rows, const int cols, const int numMines) {
  board_t* b = malloc(sizeof(board_t));
  b->r=rows;
  b->c=cols;
  b->minesLeft=numMines;
  b->visible = malloc(rows*sizeof(char*));
  b->hidden = malloc(rows*sizeof(char*));
  for (int i = 0; i < rows; i++) {
    b->visible[i] = malloc(cols*sizeof(char));
    b->hidden[i] = malloc(cols*sizeof(char));
  }
  // generate mine indices
  srand(time(0));
  int indices[numMines];
  for (int i = 0; i < numMines;) {
    int index = (rand() % (rows*cols)) + 1;
    for (int j = i; j >= 0; j--) {
      if (indices[j] == index-1) { // already mine on index
        goto skipped;
      }
    }
    indices[i] = index-1;
    i++;
    skipped:;
  }
  
  // plant mines
  int i = 0;
  int x = 0;
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      for (int j = 0; j < numMines; j++) {
        if (indices[j] == i) {
          b->hidden[r][c] = 'x';
          x++;
        }
      }
      i++;
    }
  }
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
  #ifdef DEBUG
  printf("*******     visible      *******\n");
  #endif
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
#ifdef DEBUG
  printf("******     hidden        *******\n");
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      if (board->hidden[r][c] == '\0') {
        printf("[0]\t");
      } else {
        printf("[%c]\t",board->hidden[r][c]);
      }
    }
    printf("\n");
  }
#endif
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