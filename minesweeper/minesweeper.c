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
static int handleInput(int mode, char* input, board_t* board);
static char* getModeString(int mode);

static enum modes {MANUAL, AUTO} mode;
static int MAX_QUERY_LENGTH = 5;

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
  mode = MANUAL;
  printf("\nToggle modes with '/'...currently in MANUAL");
  while (1) {
    if(boardWon(board) == 1) {
      return 0;
    }
    printf("\n%s: ",getModeString(mode));
    char input[MAX_QUERY_LENGTH];
    if (fgets(input, MAX_QUERY_LENGTH, stdin) == NULL) {
      break;
    }  
    mode = handleInput(mode, input, board);
    if (input[strlen(input)-1] != '\n') { // chop off all chars after 4 chars
      for (int ch; (ch = getchar()) != EOF && ch != '\n';)
        ;
    }
  }
  return 1;
}

static int
handleInput (int mode, char* input, board_t* board) {
    char query[MAX_QUERY_LENGTH]; 
    sscanf(input, "%s\n", query);
    char ch = '\0';
    if (strlen(query) == 1) { 
      sscanf(query, "%c", &ch);
      if ((char)ch == 'Q') { // quit
        printf("Quitter!\n");
        board_delete(board);
        exit(0);
      } else if ((char)ch == '/') { // toggle mode
        switch (mode) {
          case MANUAL:
            return AUTO;
            break;
          case AUTO:
            return MANUAL;
            break;
          default:
            fprintf(stderr,"wtf happened\n");
            board_delete(board);
            exit(69);
        } 
      } else {
        printf("Invalid key: %c\nCan use either '/' (switch mode) or 'Q' (quit)\n", ch);
        return mode;
      }
    } else if (mode == MANUAL) {
      int c = -1;
      char r='\0', ch = '\0';
      if(sscanf(query, " %c%c%d",&ch, &r, &c)!=3) {printf("Bad Query!\nFormat: <action><row><col>\n");return mode;};
      if (ch == 'f') {
        board_flag(board, r-97, c);
      } else if (ch == 'c') {
        board_click(board, r-97, c);
      } else {
        printf("Invalid key: %c\nUse either 'c' (click) or 'f' (flag)\n", ch);
        return mode;
      }
    } else if (mode == AUTO) {
      int c = -1;
      char r = '\0';
      if (sscanf(query, " %c%d", &r, &c)!=2 || r < 97) {printf("Bad Query!\nFormat: <row><col>\n");return mode;};
      board_auto(board, r-97, c);
    }
    return mode;
}

static char*
getModeString(int mode) {
  if (mode == 0) {
    return "MANUAL";
  } else {
    return "AUTO";
  }
}
