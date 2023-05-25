/*
 * minesweeper - main program for minesweeper game
 * Sam Barton - Apr 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"

static void parseArgs(const int argc, char* argv[], int* difficulty);
static int gameLoop(board_t* board);
static char* getModeString(int mode);

enum modes {MANUAL, AUTO} mode;

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
  if (*difficulty <= 0||*difficulty>3) {
    printf("usage: %s difficulty=1,2,3\n",argv[0]);
    exit(1);
  } 
}

static int
gameLoop(board_t* board) {
  mode = AUTO;
  printf("\nToggle modes with '/'...currently in MANUAL");
  while (1) {
    if(boardWon(board) == 1) {
      return 0;
    }
    printf("\n%s: ",getModeString(mode));
    char input[4];
    char query[4];
    if (fgets(input, 4, stdin) == NULL) {
      break;
    }
    sscanf(input, "%s\n", query);
    printf("\n%d",strlen(query));
    char ch = '\0';
    if (strlen(query) == 1) {
      sscanf(query, "%c", &ch);
      printf("%c", ch);
      if ((char)ch == 'Q') {
        return 0;
      } else if ((char)ch == '/') {
        //change mode
      }
    } else if (mode == MANUAL) {
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
    if (input[strlen(input)-1] != '\n') {
      for (int ch; (ch = getchar()) != EOF && ch != '\n';)
        ;
    }
  }
  return 1;
}

static char*
getModeString(int mode) {
  if (mode == 0) {
    return "MANUAL";
  } else {
    return "AUTO";
  }
}