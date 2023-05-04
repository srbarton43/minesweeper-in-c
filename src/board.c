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
#include <ncurses.h>

typedef struct board {
  int r,c;
  int minesLeft,squaresLeft;
  char** visible;
  char** hidden;
} board_t;

static void writeBoard(board_t* board);
static char getNeighbors(board_t* board, const int r, const int c);
static void zerosLogic(board_t* board, const int r, const int c);
static int touchingZero(board_t* board, const int r, const int c);
static void printHidden(board_t* board);


/*         board_create        */
board_t*
board_new (const int rows, const int cols, const int numMines) {
  board_t* b = malloc(sizeof(board_t));
  b->r=rows;
  b->c=cols;
  b->minesLeft=numMines;
  b->squaresLeft=rows*cols-numMines;
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
  for (int i = 0; i < numMines; i++) {
    b->hidden[(indices[i])/cols][-1*(indices[i]/cols)*cols+indices[i]] = 'X';
  }
  writeBoard(b);
  return b;
}

/*       board_click      */
void board_click (board_t* board, const int r, const int c) {
  #ifdef DEBUG
  printf("Clicking board at (%d,%d) ***********************\n",r,c);
  #endif
  if (board == NULL || r < 0 || r >= board->r || c < 0 || c > board->c) {
    printf("row %c, col %d is not within the board boundaries\n", (char)(97+r), c);
  } else if (board->visible[r][c] != '_') {
    printf("Cannot click at (%c,%d)!\n",(char)(97+r),c);
  } else if (board->hidden[r][c] == 'X') {
    printHidden(board);
    printf("Game Over\n");
    exit(0);
  } else if (board->hidden[r][c] == 0) {
      zerosLogic(board, r,c);
      board_print(board);
  } else {
    board->visible[r][c] = board->hidden[r][c];
    board->squaresLeft--;
    board_print(board);
  }
}

static void
zerosLogic(board_t* board, const int r, const int c) {
  if (board->hidden[r][c] == 0 || touchingZero(board, r, c)) {
    board->visible[r][c] = board->hidden[r][c];
    board->squaresLeft--;
    for (int i = r-1; i <= r+1; i++) {
      if(i<0||i>=board->r) {
        continue;
      }
      for(int j = c-1; j<=c+1; j++){
        if (j<0||j>=board->c||(i==r&&j==c)) {
          continue;
        }
        if (board->visible[i][j] == '_') {
          zerosLogic(board, i, j);
        }
      }
    }
  }
}

static int
touchingZero(board_t* board, const int r, const int c) {
  for (int i = r-1; i <= r+1; i++) {
    if (i < 0 || i >= board->r) {
      continue;
    }
    for (int j = c-1; j <= c+1; j++) {
      if (j < 0 || j >= board->c || (i==r && j==c)) {
        continue;
      } else if (board->hidden[i][j] == 0) {
        return 1;
      }
    }
  }
  return 0;
}
/*
* Writes the boards hidden values
*/
static void
writeBoard(board_t* board) {
  for (int r = 0; r < board->r; r++) {
    for (int c = 0; c < board->c; c++) {
      if (board->hidden[r][c] != 'X') {
        board->hidden[r][c] = getNeighbors(board, r, c);
      }
      board->visible[r][c] = '_';
    }
  }
}

/*
 * gets number of adjacent mines and returns char
*/
static char
getNeighbors(board_t* board, int r, int c) {
  int count = 0;
  for (int i = r-1; i <= r+1; i++) {
    if (i < 0 || i >= board->r) {
      continue;
    }
    for (int j = c-1; j <= c+1; j++) {
      if (j < 0 || j >= board->c || (i==r && j==c)) {
        continue;
      } else if (board->hidden[i][j] == 'X') {
        count++;
      }
    }
  }
  return count;
}

/*     board_flag      */
void board_flag (board_t* board, const int r, const int c) {
  #ifdef DEBUG
    printf ("Flagging square (%d,%d)   *****************************\n", r, c);
  #endif
  if (board == NULL || r < 0 || r > board->r || c < 0 || c > board->c) {
    printf("row %c, col %d is not within the board boundaries\n", (char)(r+97), c);
  } else if (board->visible[r][c] <= 9) {
    printf("Cannot flag an empty square\n");
  } else if (board->visible[r][c] == 'f') {
    board->visible[r][c] = '_';
    board->minesLeft--;
  } else {
    board->visible[r][c] = 'f';
    board->minesLeft--;
  }
  board_print(board);
}

int
boardWon(board_t* board) {
  if (board->squaresLeft == 0) {
    printHidden(board);
    return 1;
  } else {
    return 0;
  }
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
  printf("%*c Mines Left: |%d|",board->c*4-14, ' ', board->minesLeft);
  printf("\n   ");
  for (int j = 0; j < cols; j++) {
    printf(" %02d ",j);
  }
  printf("\n");
  for (int r = 0; r < rows; r++) {
    printf("%c  ",(char)(r+97));
    for (int c = 0; c < cols; c++) {
      if (board->visible[r][c] > 9) {
        printf("[%c] ",board->visible[r][c]);
      } else {
        printf("[%d] ",board->visible[r][c]);
      }
    }
    printf("\n");
  }
#ifdef DEBUG
  printHidden(board)
#endif
}

static void
printHidden(board_t* board) {
  #ifdef DEBUG
  printf("******     hidden        *******\n");
  #endif
  if (board == NULL) {
    printf("invalid board");
  }
  int rows = board->r;
  int cols = board->c;
  printf("\n   ");
  for (int j = 0; j < cols; j++) {
    printf(" %02d ",j);
  }
  printf("\n");
  for (int r = 0; r < rows; r++) {
    printf("%c  ",(char)(r+97));
    for (int c = 0; c < cols; c++) {
      int ch = board->hidden[r][c];
      if (ch > 9) {
        printf("[%c] ",ch);
      // } else if (ch == 0) {
      //   printf("[ ] ");
      } else {
        printf("[%d] ",ch);
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

#ifdef UNIT_TEST

int main (int argc, char* argv[]) {
  board_t* board = board_new(12, 20, .15*240);
  board_click(board, 1, 1);
  board_click(board ,2,2);
  board_flag(board, 2, 3);
  board_flag(board, 3, 4);
  board_flag(board, 10, 3);
  board_flag(board, 2, 3);
  board_delete(board);
}
#endif
