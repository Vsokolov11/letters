#ifndef UTILS_H
#define UTILS_H

#include <ncurses.h>
#include <dirent.h>
#include <stdio.h>

void create_colors();
void print_bottom(const char *format, ...);
void destroy_win(WINDOW *local_win);

#endif
