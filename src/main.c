#include <ncurses.h>
#include <dirent.h>

#include "utils.h"
#include "file_manager.h"

int main()
{
  initscr();
  cbreak();

  create_colors();
  keypad(stdscr, TRUE);

  init_searchbox();

  // getch();
  endwin();

  return 0;
}
