#include "text_editor.h"

#include "file_manager.h"
#include "utils.h"

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void draw_screen(fb_buffer *buf, int cursor_x, int cursor_y, int scroll_offset) {
  clear();
  int screen_rows = LINES - 1;

  fb_line *tmp = buf->head;
  for (int i = 0; i < scroll_offset && tmp; i++) {
    tmp = tmp->next;
  }

  int row = 0;
  while (tmp && row < screen_rows) {
    fb_render_line(tmp, row++);
    tmp = tmp->next;
  }

  move(cursor_y - scroll_offset, cursor_x);
  refresh();
}
int open_file_editor() {
  const char *path = init_searchbox();
  if (!path) return -1;

  fb_buffer *buf = load_file_in_buffer(path);
  char *current_path = strdup(path);
  free((char*)path);

  if (!buf) return -1;

  refresh();
  curs_set(1);
  noecho();
  raw();
  keypad(stdscr, TRUE);

  int cursor_x = 0;
  int cursor_y = 0;
  int scroll_offset = 0;
  fb_line *current_line = buf->head;
  int ch;

  fb_move_gap(current_line, 0);

  bool running = true;
  bool confirm_quit = false;

  draw_screen(buf, cursor_x, cursor_y, scroll_offset);

  while (running) {
    ch = getch();

    if (ch != CTRL('q')) confirm_quit = false;

    bool full_redraw = false;

    switch (ch) {
      case KEY_UP:
        if (current_line->prev) {
          current_line = current_line->prev;
          cursor_y--;

          int len = current_line->gap_start + (current_line->size - current_line->gap_end);
          if (cursor_x > len) cursor_x = len;

          fb_move_gap(current_line, cursor_x);
        }
        break;
      case KEY_DOWN:
        if (current_line->next) {
          current_line = current_line->next;
          cursor_y++;

          int len = current_line->gap_start + (current_line->size - current_line->gap_end);
          if (cursor_x > len) cursor_x = len;
          fb_move_gap(current_line, cursor_x);
        }
        break;
      case KEY_LEFT:
        if (cursor_x > 0) {
          cursor_x--;
          fb_move_gap(current_line, cursor_x);
        }
        break;
      case KEY_RIGHT:
        ;int len = current_line->gap_start + (current_line->size - current_line->gap_end);
        if (cursor_x < len) {
          cursor_x++;
          fb_move_gap(current_line, cursor_x);
        }
        break;
      case 10: // Enter
        fb_split_line(buf, current_line);
        current_line = current_line->next;
        cursor_y++;
        cursor_x = 0;
        fb_move_gap(current_line, 0);
        buf->is_dirty = true;
        full_redraw = true;
        break;
      case KEY_BACKSPACE:
      case 127:
          // Handle deletion logic here
          // set full_redraw = true when joining lines
          // buf->is_dirty = true;
          break;
      case CTRL('o'):
        const char *new_path = init_searchbox();

        if (new_path) {
          fb_free(buf);
          buf = load_file_in_buffer(new_path);
          free(current_path);
          current_path = strdup(new_path);
          free((char*)new_path);
          cursor_x = 0;
          cursor_y = 0;
          scroll_offset = 0;
          current_line = buf->head;
          fb_move_gap(current_line, 0);
        }

        full_redraw = true;
        break;
      case CTRL('s'):
        if (save_file(buf, current_path) == 0) {
          buf->is_dirty = false;
          print_bottom(1, "File saved successfully.");
        }
        break;
      case CTRL('q'):
        if (!buf->is_dirty || confirm_quit) {
          running = false;
        } else {
          confirm_quit = true;
          print_bottom(3, "Unsaved changes! Press Ctrl+Q again to quit.");
        }
        break;
      default:
        if (ch >= 32 && ch <= 126) {
          fb_insert_char(current_line, ch);
          buf->is_dirty = true;
          cursor_x++;
        }
        break;
    }

    if (cursor_y >= scroll_offset + LINES - 1) { scroll_offset++; full_redraw = true; }
    if (cursor_y < scroll_offset) { scroll_offset--; full_redraw = true; }

    //offset might have to be recalculated if jumping more than one line at a time
    if (full_redraw) {
      draw_screen(buf, cursor_x, cursor_y, scroll_offset);
    } else {
      fb_render_line(current_line, cursor_y - scroll_offset);
    }

    move(cursor_y - scroll_offset, cursor_x);
    refresh();
  }

  if (buf != NULL) fb_free(buf);
  return 0;
}

void fb_render_line(fb_line *line, int row) {
    move(row, 0);
    clrtoeol();

    for (int i = 0; i < line->gap_start; i++) {
        addch(line->data[i]);
    }

    for (int i = line->gap_end; i < line->size; i++) {
        addch(line->data[i]);
    }
}

fb_buffer* load_file_in_buffer(const char *path) {
  FILE *fp = fopen(path, "r");

  if (fp == NULL) {
    fb_buffer *buf = fb_create();
    fb_insert_line(buf, NULL, "");
    return buf;
  }

  fb_buffer *buf = fb_create();
  char *line_ptr = NULL;
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line_ptr, &len, fp)) != -1) {
    if (read > 0 && line_ptr[read - 1] == '\n') {
      line_ptr[read - 1] = '\0';
    }

    fb_insert_line(buf, buf->tail, line_ptr);
  }

  if (buf->total_lines == 0) {
    fb_insert_line(buf, NULL, "");
  }

  free(line_ptr);
  fclose(fp);
  return buf;
}

int save_file(fb_buffer *buf, const char *path) {
  FILE *fp = fopen(path, "w");
  if (!fp) return -1;

  fb_line *line = buf->head;
  while (line) {
    for (int i = 0; i < line->gap_start; i++)
      fputc(line->data[i], fp);
    for (int i = line->gap_end; i < line->size; i++)
      fputc(line->data[i], fp);
    fputc('\n', fp);
    line = line->next;
  }

  fclose(fp);
  return 0;
}
