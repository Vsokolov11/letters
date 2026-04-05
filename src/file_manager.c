#include "file_manager.h"

#include <dirent.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>

#include "utils.h"
#include "path_list.h"
// #include "text_editor.h"

void handle_path (path_list *list, const char *path) {
  if (chdir(path) != 0) {
    print_bottom("folder cannot be accessed: %s", path);

    return;
  }

  path_list_free(list);
  path_list_init(list, 32);

  DIR *dirp = opendir(".");
  if (dirp == NULL) return;

  struct dirent *entry;
  while ((entry = readdir(dirp)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0) continue;

    PathEntry fe;
    strncpy(fe.name, entry->d_name, sizeof(fe.name) - 1);
    fe.name[sizeof(fe.name) - 1] = '\0';

    fe.is_dir = (entry->d_type == DT_DIR);

    path_list_add(list, fe);
  }

  closedir(dirp);
}

int init_searchbox() {
    char str[PATH_MAX];
    int height = LINES / 2;
    int width = COLS / 2;
    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;

    curs_set(0);

    WINDOW *search_win = newwin(height, width, starty, startx);
    keypad(search_win, TRUE);

    path_list file_list = {};
    handle_path(&file_list, ".");

    int highlight = 0;
    int scroll_offset = 0;
    int visible_rows = height - 3;

    bool exploring = true;

    while (exploring) {
      wclear(search_win);
      box(search_win, 0, 0);
      noecho();

      if (highlight >= scroll_offset + visible_rows) {
        scroll_offset = highlight - visible_rows + 1;
      }
      if (highlight < scroll_offset) {
        scroll_offset = highlight;
      }

      for (int i = 0; i < visible_rows && (i + scroll_offset) < file_list.size; i++) {
        int curr_idx = i + scroll_offset;
        PathEntry *current = &file_list.entries[curr_idx];

        if (curr_idx == highlight) {
          wattron(search_win, A_REVERSE);
        }

        if (current->is_dir) {
          wattron(search_win, A_BOLD);
          mvwprintw(search_win, i + 2, 1, "%s/", current->name);
          wattroff(search_win, A_BOLD);
        } else {
          mvwprintw(search_win, i + 2, 1, "%s", current->name);
        }
        wattroff(search_win, A_REVERSE);
      }

      wrefresh(search_win);

      int ch = wgetch(search_win);

      switch (ch) {
        case KEY_UP:
          if (highlight > 0) highlight--;
          break;
        case KEY_DOWN:
          if (highlight < file_list.size - 1) highlight++;
          break;
        case 10:
          if (file_list.entries[highlight].is_dir) {
            handle_path(&file_list, file_list.entries[highlight].name);
            highlight = 0;
            scroll_offset = 0;
          } else {
            // open_file_editor(file_list.entries[highlight].name);
            // I may need to refresh something here .
          }

          char cwd[PATH_MAX];

          if (getcwd(cwd, sizeof(cwd)) != NULL) {
            print_bottom("%s\n", cwd);
          }

          break;
        case 'q':
          exploring = false;
          break;
      }

      // break;
    }

    path_list_free(&file_list);
    delwin(search_win);
    return 0;
}
