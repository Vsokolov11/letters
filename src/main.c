#include <ncurses.h>
#include <dirent.h>

#include "utils.h"
#include "text_editor.h"

int main()
{
  initscr();
  cbreak();

  create_colors();
  keypad(stdscr, TRUE);

  open_file_editor();

  // getch();
  endwin();

  return 0;
}
