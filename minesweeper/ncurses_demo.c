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
static void updateDisplay(board_t* board, const char* status, const char* map, const coords_t* pos);
static void write_board(board_t* board, const coords_t* curPos);
static void colorNumber(coords_t* pos, char c);

static void print_tile(void* arg, char c);
static void nextRow(void* arg);

typedef struct iterateOpts {
  const coords_t* curPos;
  coords_t* pos;
} iterateOpts_t;

int main(int argc, char* argv[]) {
  board_t* board = board_new(9,9,3);
  ncurse_init();
  char str[50];
  char status[20];
  coords_t* pos = malloc(sizeof(coords_t));
  pos->x=0; pos->y=0;
  while (true) {
    updateDisplay(board, status, str, pos);
    handleInput(board, status, pos);
  }
  free(pos);
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
handleInput(board_t* board, char* status, coords_t* pos) {
  sprintf(status, "");
  int c = getch();
  int r = 0;
  switch (c) {
    case 'h':
      if (pos->x > 0) {
        pos->x-=4;
      }
      break;
    case 'l':
      if (pos->x < 8*4) {
        pos->x+=4;
      }
      break;
    case 'j':
      if (pos->y < 8) {
        pos->y++;
      }
      break;
    case 'k':
      if (pos->y > 0) {
        pos->y--;
      }
      break;
    case 'x':
      board_click(board, pos->y, (pos->x)/4);
      r = 1;
      break;
    case 'f':
      board_flag(board, pos->y, (pos->x)/4);
      r=1;
      break;
    case 'Q':
      endwin();
      board_print(board);
      board_delete(board);
      free(pos);
      exit(0);
      break;
    default:
      ;
  }
  if (r) {
    sprintf(status, "clicked: (%d, %d)",(pos->y), (pos->x)/4);
  } else {
    sprintf(status, "key: %c\tpos: (%d,%d)", c, (pos->x)/4,pos->y);
  }
}

static void write_board(board_t* board, const coords_t* curPos) {
  move(1, 0);
  coords_t* pos = malloc(sizeof(coords_t));
  pos->x=0; pos->y=1;
  
  iterateOpts_t opts;
  opts.pos = pos, opts.curPos = curPos;
  board_iterate(board, &opts, print_tile, NULL, nextRow);
  free(pos);
}

static void 
print_tile(void* arg, char c) {
  iterateOpts_t* opts = (iterateOpts_t*) arg;
  coords_t* pos = opts->pos;
  const coords_t* curPos = opts->curPos;
  char temp[10];
  if (curPos->x == pos->x && curPos->y == pos->y-1) {
    if (c == '_') {
      sprintf(temp, "_");
    } else if (c == 'f' || c > 9) {
      sprintf(temp, "%c", c);
    } else {
      sprintf(temp, "%d", c);
    }
    attron(COLOR_PAIR(2));
    mvprintw(pos->y, pos->x, "(");
    attron(COLOR_PAIR(1));
    mvprintw(pos->y, pos->x+1, temp);
    attron(COLOR_PAIR(2));
    mvprintw(pos->y, pos->x+2, ")");
    attron(COLOR_PAIR(1));
  } else {
    if (c == '_') {
      sprintf(temp, " _  ");
      mvprintw(pos->y, pos->x, temp);
    } else if (c == 'f') {
      sprintf(temp, "[%c] ", c);
      attron(COLOR_PAIR(2));
      mvprintw(pos->y, pos->x, temp);
      attron(COLOR_PAIR(1));
    } else if (c > 9) {
      sprintf(temp, "[%c] ", c);
      mvprintw(pos->y, pos->x, temp);
      // mvprintw(pos->y, pos->x, "[");
      // colorNumber(pos, c);
      // mvprintw(pos->y, pos->x+2, "] ");
      // pos->x-=2;
    } else {
      sprintf(temp, "[%d] ", c);
      mvprintw(pos->y, pos->x, "[");
      colorNumber(pos, c);
      mvprintw(pos->y, pos->x+2, "] ");
    }
  }
  pos->x+=4;
}

static void 
nextRow(void* arg) {
  iterateOpts_t* opts = (iterateOpts_t*) arg;
  coords_t* pos = opts->pos;
  pos->x=0; pos->y++;
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
  sprintf(temp, "%d", c);
  mvprintw(pos->y, pos->x+1, temp);
  attron(COLOR_PAIR(1));
}

static void
updateDisplay(board_t* board, const char* status, const char* map, const coords_t* pos) {
  clrtoeol();
  mvprintw(0,0, status);
  write_board(board, pos);
  move(pos->y+1, pos->x+1);
  refresh();
}
