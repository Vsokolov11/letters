#include "utils.h"
#include <ncurses.h>

void create_colors() {
 	start_color();

  init_pair(1, COLOR_BLACK, COLOR_RED);
  init_pair(2, COLOR_CYAN, COLOR_BLACK);
}

void print_bottom(const char *format, ...) {
    va_list args;

    va_start(args, format);

    move(LINES - 1, 0);
    clrtoeol();

    vw_printw(stdscr, format, args);

    va_end(args);

    refresh();
}

void destroy_win(WINDOW *local_win)
{
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');

	wrefresh(local_win);
	delwin(local_win);
}
