#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "board.h"

typedef struct coords {
  int x;
  int y;
} coords_t;

static void ncurse_init();
static void handleInput(board_t* board, char* status, coords_t* pos);
static void updateDisplay(board_t* board, const char* status, const char* map, const coords_t* cursPos);
static void write_board(board_t* board, const coords_t* cursPos);
static void colorNumber(coords_t* pos, char c);
static void print_tile(void* arg, char c);
static void nextRow(void* arg);

typedef struct iterateOpts {
  const coords_t* cursPos;
  coords_t* pos;
} iterateOpts_t;

int main(int argc, char* argv[]) {
  board_t* board = board_new(9,9,3);
  ncurse_init();
  char str[50];
  char status[20];
  coords_t* cursPos = malloc(sizeof(coords_t));
  cursPos->x=0; cursPos->y=0;
  while (true) {
    updateDisplay(board, status, str, cursPos);
    handleInput(board, status, cursPos);
    if (boardWon(board)) {
      sprintf(status, "You won?");
    }
  }
}

static void
ncurse_init()
{
  initscr();
  cbreak();
  noecho();
  start_color();
  curs_set(0);
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_YELLOW, COLOR_BLACK);
  init_pair(3, COLOR_CYAN, COLOR_BLACK);
  init_pair(4, COLOR_GREEN, COLOR_BLACK);
  init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(6, COLOR_BLUE, COLOR_BLACK);
  attron(COLOR_PAIR(1));
}

static void
handleInput(board_t* board, char* status, coords_t* cursPos) {
  sprintf(status, "");
  int c = getch();
  int r = 0;
  switch (c) {
    case 'h':
      if (cursPos->x > 0) {
        cursPos->x-=4;
      }
      break;
    case 'l':
      if (cursPos->x < 8*4) {
        cursPos->x+=4;
      }
      break;
    case 'j':
      if (cursPos->y < 8) {
        cursPos->y++;
      }
      break;
    case 'k':
      if (cursPos->y > 0) {
        cursPos->y--;
      }
      break;
    case 'x':
      if (board_click(board, cursPos->y, (cursPos->x)/4)) {
        endwin();
        board_delete(board);
        free(cursPos);
        printf("You Lose\n");
        exit(0);
      }
      r = 1;
      break;
    case 'f':
      board_flag(board, cursPos->y, (cursPos->x)/4);
      r=1;
      break;
    case ' ':
      board_auto(board, cursPos->y, (cursPos->x)/4);
      r = 1;
      break;
    case 'Q':
      endwin();
      board_print(board);
      board_delete(board);
      free(cursPos);
      exit(0);
      break;
    default:
      ;
  }
  if (r) {
    sprintf(status, "clicked: (%d, %d)",(cursPos->y), (cursPos->x)/4);
  } else {
    sprintf(status, "key: %c\tcursPos: (%d,%d)", c, (cursPos->x)/4,cursPos->y);
  }
}

static void write_board(board_t* board, const coords_t* cursPos) {
  for (int r = 0; r <= 8; r++) {
    char hdrNum[3];
    sprintf(hdrNum, "%02d", r);
    mvprintw(1, r*4+3+1, hdrNum);
  }
  coords_t* pos = malloc(sizeof(coords_t));
  pos->x=0; pos->y=1;
  iterateOpts_t opts;
  opts.pos = pos, opts.cursPos = cursPos;
  board_iterate(board, &opts, print_tile, NULL, nextRow);
  free(pos);
}

static void 
print_tile(void* arg, char c) {
  iterateOpts_t* opts = (iterateOpts_t*) arg;
  coords_t* pos = opts->pos;
  const coords_t* cursPos = opts->cursPos;
  char temp[10];
  if (cursPos->x == pos->x && cursPos->y == pos->y-2) {
    if (c == '_') {
      sprintf(temp, "_");
    } else if (c == 'f' || c > 9) {
      sprintf(temp, "%c", c);
    } else {
      sprintf(temp, "%d", c);
    }
    attron(COLOR_PAIR(2));
    mvprintw(pos->y, pos->x+3, "(");
    attron(COLOR_PAIR(1));
    mvprintw(pos->y, pos->x+4, temp);
    attron(COLOR_PAIR(2));
    mvprintw(pos->y, pos->x+5, ")");
    attron(COLOR_PAIR(1));
  } else {
    if (c == '_') {
      sprintf(temp, " _  ");
      mvprintw(pos->y, pos->x+3, temp);
    } else if (c == 'f') {
      sprintf(temp, "[%c] ", c);
      attron(COLOR_PAIR(2));
      mvprintw(pos->y, pos->x+3, temp);
      attron(COLOR_PAIR(1));
    } else if (c > 9) {
      sprintf(temp, "[%c] ", c);
      mvprintw(pos->y, pos->x+3, temp);
      // mvprintw(pos->y, pos->x, "[");
      // colorNumber(pos, c);
      // mvprintw(pos->y, pos->x+2, "] ");
      // pos->x-=2;
    } else {
      sprintf(temp, "[%d] ", c);
      colorNumber(pos, c);
      // Old Vers
      // mvprintw(pos->y, pos->x, "[");
      // colorNumber(pos, c);
      // mvprintw(pos->y, pos->x+2, "] ");
    }
  }
  pos->x+=4;
}

static void 
nextRow(void* arg) {
  iterateOpts_t* opts = (iterateOpts_t*) arg;
  coords_t* pos = opts->pos;
  pos->x=0; pos->y++;
  char rowChar[5];
  sprintf(rowChar, "%c  ", pos->y+65-2);
  mvprintw(pos->y, 0, rowChar);
}

static void
colorNumber(coords_t* pos, char c) {
  if (c == '0') {
    ;
  } else if (c == 1) {
    attron(COLOR_PAIR(3));
  } else if (c == 2) {
    attron(COLOR_PAIR(4));
  } else if (c == 3) {
    attron(COLOR_PAIR(5));
  } else if (c == 4) {
    attron(COLOR_PAIR(6));
  }
  char temp[5];
  sprintf(temp, "[%d]", c);
  mvprintw(pos->y, pos->x+3, temp);
  attron(COLOR_PAIR(1));
}

static void
updateDisplay(board_t* board, const char* status, const char* map, const coords_t* cursPos) {
  move(0,0);
  clrtoeol();
  mvprintw(0,0, status);
  write_board(board, cursPos);
  refresh();
}
