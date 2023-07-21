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
static void write_board(board_t* board);

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
  init_pair(1, COLOR_YELLOW, COLOR_BLACK);
  attron(COLOR_PAIR(1));
}

static void
handleInput(board_t* board, char* status, coords_t* pos) {
  sprintf(status, "");
  int c = getch();
  int r = 0;
  switch (c) {
    case 'h':
      pos->x-=5;
      break;
    case 'l':
      pos->x+=5;
      break;
    case 'j':
      pos->y++;
      break;
    case 'k':
      pos->y--;
      break;
    case 'x':
      board_click(board, pos->y, (pos->x)/5);
      r = 1;
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
    sprintf(status, "clicked: (%d, %d)",(pos->y), (pos->x)/5);
  } else {
    sprintf(status, "key: %c\tpos: (%d,%d)", c, (pos->x)/5,pos->y);
  }
}

static void write_board(board_t* board) {
  move(1, 0);
  coords_t* pos = malloc(sizeof(coords_t));
  pos->x=0; pos->y=1;
  for (int r = 0; r < board->r; r++) {
    for (int c = 0; c < board->c; c++) {
      char ch = board->visible[r][c];
      char temp[10];
      if (ch == 'f') {
        sprintf(temp, "[%c] ", ch);
      } else if (ch == '_') {
        sprintf(temp, " _  ");
      } else if (ch > 9) {
        sprintf(temp, "[%c] ", ch);
      } else {
        sprintf(temp, "[%d] ", ch);
      }
      mvprintw(pos->y, pos->x, temp);
      pos->x+=5;
    }
    pos->x=0; pos->y++;
  }
  free(pos);
}

static void
updateDisplay(board_t* board, const char* status, const char* map, const coords_t* pos) {
  clrtoeol();
  mvprintw(0,0, status);
  write_board(board);
  move(pos->y+1, pos->x+1);
  refresh();
}
