#ifndef UTILS_H
#define UTILS_H

#include <ncurses.h>
#include <dirent.h>

#define CTRL(c) ((c) & 0x1f)

void create_colors();
void print_bottom(int severity, const char *format, ...);
void print_logo();
void destroy_win(WINDOW *local_win);

#endif
