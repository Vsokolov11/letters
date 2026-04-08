#ifndef FILE_BUFFER_H
#define FILE_BUFFER_H

#include <stdbool.h>

typedef struct fb_line {
  char *data;
  int gap_start;
  int gap_end;
  int size;

  struct fb_line *next;
  struct fb_line *prev;
} fb_line;

typedef struct {
  fb_line *head;
  fb_line *tail;
  int total_lines;
  bool is_dirty;
} fb_buffer;

fb_buffer* fb_create();
void fb_insert_line(fb_buffer *buf, fb_line *after, const char *text);
void fb_insert_char(fb_line *line, char c);
void fb_split_line(fb_buffer *buf, fb_line *current_line);
void fb_move_gap(fb_line *line, int position);
void fb_free(fb_buffer *buf);

#endif
