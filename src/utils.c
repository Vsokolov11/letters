#include "utils.h"
#include <ncurses.h>
#include <stdio.h>
#include <string.h>

void create_colors() {
 	start_color();

  init_pair(1, COLOR_BLACK, COLOR_GREEN);
  init_pair(2, COLOR_BLACK, COLOR_YELLOW);
  init_pair(3, COLOR_WHITE, COLOR_RED);
  init_pair(4, COLOR_CYAN, COLOR_BLACK);
}

void print_bottom(int severity, const char *format, ...) {
    va_list args;

    va_start(args, format);

    move(LINES - 1, 0);
    clrtoeol();

    wattron(stdscr, COLOR_PAIR(severity));
    vw_printw(stdscr, format, args);
    wattroff(stdscr, COLOR_PAIR(severity));

    va_end(args);

    refresh();
}

void print_logo() {
  const char *logo[] = {
  "  _      _   _                ",
  " | | ___| |_| |_ ___ _ __ ___ ",
  " | |/ _ \\ __| __/ _ \\ '__/ __|",
  " | |  __/ |_| ||  __/ |  \\__ \\",
  " |_|\\___|\\__|\\__\\___|_|  |___/ ",
  "", NULL
  };

  for (int i = 0; logo[i] != NULL; i++) {
    mvprintw(i, (COLS - strlen(logo[0])) / 2, "%s", logo[i]);
  }
  refresh();
}

void destroy_win(WINDOW *local_win)
{
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');

	wrefresh(local_win);
	delwin(local_win);
}
