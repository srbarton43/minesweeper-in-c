/*
 * board.c - source code for board module
 *
 * See header file for function info
 *
 *
 * Sam Barton - April 2023
 */

#include "board.h"
#include <stdio.h>
#include <time.h>
#include <stdbool.h>


typedef struct board {
  int r,c;
  int minesLeft,squaresLeft;
  char** visible;
  char** hidden;
  bool empty;
} board_t;

/*      static function prototypes             */
static void writeBoard(board_t* board);
static char getNeighbors(board_t* board, const int r, const int c);
static void firstClick(board_t* board, const int r, const int c);
static void zerosLogic(board_t* board, const int r, const int c);
static int touchingZero(board_t* board, const int r, const int c);
static void printHidden(board_t* board, const int status);
static void getColor(const int i);
static void iterateNeighbors(board_t* board, const int r, const int c, void* arg,
                             void (*iteratefunc)(void* arg, board_t* board, int r, int c));

/*       iteratefunc's      */
static void countFlags(void* arg, board_t* board, const int r, const int c);
static void countMines(void* arg, board_t* board, const int r, const int c);
static void clickNeighbors(void* arg, board_t* board, const int r, const int c);
static void isZero(void* arg, board_t* board, const int r, const int c);
static void recurseZeroes(void* arg, board_t* board, const int r, const int c);

/*    color prototypes     */
static void reset(void);
static void red(void);
static void green(void);
static void yellow(void);
static void blue(void);
static void purple(void);
static void cyan(void);


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
    for (int k = 0; k < rows; k++) {
      b->visible[i][k] = '0';
      b->hidden[i][k] = '0';
    }
  }
  b->empty = true;
  return b;
}

/*       board_click      */
void 
board_click (board_t* board, const int r, const int c) {
  #ifdef DEBUG
  printf("\nClicking board at (%d,%d) ***********************\n",r,c);
  #endif
  if (board == NULL || r < 0 || r >= board->r || c < 0 || c > board->c) {
    printf("row %c, col %d is not within the board boundaries\n", (char)(65+r), c);
  } else if (board->visible[r][c] != '0') {
    printf("Cannot click at (%c,%d)!\n",(char)(65+r),c);
  } else if (board->empty) {
    board->empty = false;
    firstClick(board, r, c);
  } else if (board->hidden[r][c] == 'X') {
    board->hidden[r][c]='#';
    red();
    printf("%*c",board->c*4/2-3,' ');printf("___________");
    printf("\n%*c|*Game Over*|",board->c*4/2-4,' ');
    printf("\n%*c",board->c*4/2-3,' ');printf("-----------");
    reset();
    printHidden(board,0);
    red();
    printf("\n%*c",board->c*4/2-3,' ');printf("___________");
    printf("\n%*c|*Game Over*|",board->c*4/2-4,' ');
    printf("\n%*c",board->c*4/2-3,' ');printf("-----------\n"); 
    reset();
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

// first click on empty board
// must generate mines after the click
// assumes that clicked tile is valid since already checked in board_click
static void
firstClick(board_t* board, const int r, const int c) {
  // generate mine indices
  int clickIndex = r*board->c + c + 1;
#ifdef DEBUG
  printf("####### MINE GEN DEBUG #######\n");
  printf("first click index: %d\n", clickIndex);
#endif
  srand(time(0));
  int indices[board->minesLeft];
  for (int i = 0; i < board->minesLeft;) {
    int index = (rand() % (board->r*board->c)) + 1;
#ifdef DEBUG
    printf("mine index: %d\n", index);
#endif
    if (index == clickIndex) {
#ifdef DEBUG
      printf("skipped CI=%d, I=%d\n",clickIndex, index);
#endif
      goto skipped; // cant be a mine on first click
    }
    for (int j = i; j >= 0; j--) {
      if (indices[j] == index-1) { // already mine on index
#ifdef DEBUG
        printf("skipping index %d\n", index);
#endif
        goto skipped;
      }
    }
    indices[i] = index-1;
    i++;
    skipped:;
  }
  
  // plant mines
  for (int i = 0; i < board->minesLeft; i++) {
    board->hidden[(indices[i])/board->c][-1*(indices[i]/board->c)*board->c+indices[i]] = 'X';
  }
  writeBoard(board);
  board_click(board, r, c);
}

// recursive check to clear zero-neighboring tiles
static void
zerosLogic(board_t* board, const int r, const int c) {
  if (board->hidden[r][c] == 0 || touchingZero(board, r, c)) {
    if (board->hidden[r][c] == 0) {
      board->visible[r][c] = '_';
    } else {
      board->visible[r][c] = board->hidden[r][c];
    }
    board->squaresLeft--;
    iterateNeighbors(board, r, c, NULL, recurseZeroes);
  }
}

// check if tile is neighboring zero tile
static int
touchingZero(board_t* board, const int r, const int c) {
  int val = 0;
  iterateNeighbors(board, r, c, &val, isZero);
  return val;
}
  
// returns 1 if tile is zero
static void
isZero(void* arg, board_t* board, const int r, const int c) {
  int* val = arg;
  if (board->hidden[r][c] == 0) {
    *val = 1;
  }
}

static void
recurseZeroes(void* arg, board_t* board, const int r, const int c) {
  if (board->visible[r][c] == '0') {
    zerosLogic(board, r, c);
  }    
}

void board_auto(board_t* board, const int r, const int c) {
  #ifdef DEBUG
  printf("\nAuto-Clicking board at (%d,%d) ***********************\n",r,c);
  #endif
  if (board == NULL || r < 0 || r >= board->r || c < 0 || c > board->c) {
    printf("row %c, col %d is not within the board boundaries\n", (char)(65+r), c);
  }
  if (board->visible[r][c] == '0' || board->visible[r][c] == 'f') {
    board_flag(board, r, c);
  } else {
    int flagCount = 0;
    iterateNeighbors(board, r, c, &flagCount, countFlags);
    if (flagCount >= board->visible[r][c]) {
      iterateNeighbors(board, r, c, NULL, clickNeighbors);
    } else {
      board_print(board);
    }
  }
}

// iteratorfunc to get flag count
static void 
countFlags(void* arg, board_t* board, const int r, const int c) {
  int* count = arg;
  if (board->visible[r][c] == 'f') {(*count)++;}
}

// iteratorfunc to click neighbors
static void
clickNeighbors(void* arg, board_t* board, const int r, const int c) {
  if (board->visible[r][c] == '0') {board_click(board, r, c);}
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
    }
  }
}

/*
 * gets number of adjacent mines and returns char
*/
static char
getNeighbors(board_t* board, int r, int c) {
  int count = 0;
  iterateNeighbors(board, r, c, &count, countMines);
  return count;
}

static void
countMines(void* arg, board_t* board, const int r, const int c) {
  int* count = arg;
  if (board->hidden[r][c] == 'X') {(*count)++;}
}

/*     board_flag      */
void board_flag (board_t* board, const int r, const int c) {
  #ifdef DEBUG
    printf ("\nFlagging square (%d,%d)   *****************************\n", r, c);
  #endif
  if (board == NULL || r < 0 || r > board->r || c < 0 || c > board->c) {
    printf("row %c, col %d is not within the board boundaries\n", (char)(r+65), c);
  } else if (board->visible[r][c] <= 9 || board->visible[r][c] == '_') {
    printf("Cannot flag an empty square\n");
  } else if (board->visible[r][c] == 'f') {
    board->visible[r][c] = '0';
    board->minesLeft++;
  } else {
    board->visible[r][c] = 'f';
    board->minesLeft--;
  }
  board_print(board);
}

/*      boardWon     */
int
boardWon(board_t* board) {
  if (board->squaresLeft == 0) {
    green();
    printf("%*c",board->c*4/2-3,' ');printf("___________");
    printf("\n%*c|*You Won!*|",board->c*4/2-4,' ');
    printf("\n%*c",board->c*4/2-3,' ');printf("-----------");
    reset();
    printHidden(board,1);
    green();
    printf("%*c",board->c*4/2-3,' ');printf("__________");
    printf("\n%*c|*You Won!*|",board->c*4/2-4,' ');
    printf("\n%*c",board->c*4/2-3,' ');printf("----------\n");
    reset();
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
  printf("\n*******     visible      *******\n");
  #endif
  printf("\n%*c Mines Left: |%d|",board->c*4-14, ' ', board->minesLeft);
  printf("\n   ");
  for (int j = 0; j < cols; j++) {
    printf(" %02d ",j);
  }
  printf("\n");
  for (int r = 0; r < rows; r++) {
    printf("%c  ",(char)(r+65));
    for (int c = 0; c < cols; c++) {
      char ch = board->visible[r][c];
      if (ch == 'f') {
        yellow();
        printf("[");
        printf("%c",ch);
        printf("] ");
        reset();
      } else if (board->visible[r][c] == '_') {
        printf(" _  ");
      } else if (ch > 9) {
        printf("[%c] ", ch);
      } else {
        getColor(ch);
        printf("[");
        printf("%d",ch);
        printf("] ");
        reset();
      }
    }
    printf("\n");
  }
#ifdef DEBUG
  printHidden(board,0);
#endif
}

static void 
getColor(const int i) {
  switch (i) {
    case 0:
      reset();
      break;
    case 1:
      cyan();
      break;
    case 2:
      green();
      break;
    case 3:
      purple();
      break;
    default:
      blue();
      break;
  }
}

static void
printHidden(board_t* board, const int status) {
  #ifdef DEBUG
  printf("\n******     hidden        *******\n");
  #endif
  if (board == NULL) {
    printf("invalid board");
  }
  int rows = board->r;
  int cols = board->c;
  if (status == 1) {
    printf("\n%*c Mines Left: |%d|",board->c*4-14, ' ',0);
  } else {
    printf("\n%*c Mines Left: |%d|",board->c*4-14, ' ', board->minesLeft);
  }
  printf("\n   ");
  for (int j = 0; j < cols; j++) {
    printf(" %02d ",j);
  }
  printf("\n");
  for (int r = 0; r < rows; r++) {
    printf("%c  ",(char)(r+65));
    for (int c = 0; c < cols; c++) {
      int ch = board->hidden[r][c];
      if (ch == 'X') {
        if (status == 0) {
          yellow();
          printf("[");
          red();
          printf("%c",ch);
          yellow();
          printf("] ");
          reset();
        } else {
          red();
          printf("[");
          yellow();
          printf("f");
          red();
          printf("] ");
          reset();
        }
      // } else if (ch == 0) {
      //   printf("[ ] ");
      } else if (ch == '#') {
        red();
        printf("{");
        yellow();
        printf("#");
        red();
        printf("} ");
        reset();
      } else if (ch == 0) {
        printf(" _  ");
      } else {
        getColor(ch);
        printf("[%d] ",ch);
        reset();
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

/*        color definitions            */
static void reset(void){printf("\033[0;0m");}
static void red(void){printf("\033[0;31m");}
static void green(void){printf("\033[0;32m");}
static void yellow(void){printf("\033[0;33m");}
static void blue(void){printf("\033[0;34m");}
static void purple(void){printf("\033[0;35m");}
static void cyan(void){printf("\033[0;36m");}

#ifdef UNIT_TEST
void sumHidden(void* arg, board_t* board, int r, int c) {
  int* sum = arg;
  if (sum != NULL && board != NULL) {
    printf("%d\t",board->hidden[r][c]);
    if (board->hidden[r][c] != 'X')
      *sum+=board->hidden[r][c];
  }
}


int main (int argc, char* argv[]) {
  board_t* board = board_new(12, 20, .15*240);
  board_click(board, 1, 1);
  board_click(board ,2,2);
  board_flag(board, 2, 3);
  board_flag(board, 3, 4);
  board_flag(board, 10, 3);
  board_flag(board, 2, 3);
  board_print(board);
  int sum = 0;
  printf("getting sum(5,5)\n");
  iterateNeighbors(board, 5, 5, &sum, sumHidden);
  printf("%d\n", sum);

  board_delete(board);
}


#endif

/*
 * iterate neighbors of r,c and call iteratefunc on it
 */
static void
iterateNeighbors(board_t* board, const int r, const int c, void* arg,
                 void (*iteratefunc)(void* arg, board_t* board, int r, int c)) {
  if (board == NULL) return;
  for (int i = r-1; i <= r+1; i++) {
    if (i < 0 || i >= board->r) {
      continue;
    }
    for (int j = c-1; j <= c+1; j++) {
      if (j < 0 || j >= board->c || (i==r && j==c)) {
        continue;
      }
      (*iteratefunc)(arg, board, i, j);
    }
  } 
}
