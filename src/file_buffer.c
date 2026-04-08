#include "file_buffer.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define INITIAL_GAP_SIZE 16

fb_buffer* fb_create() {
  fb_buffer *buf = malloc(sizeof(fb_buffer));
  if (!buf) return NULL;

  buf->head = NULL;
  buf->tail = NULL;

  buf->total_lines = 0;

  return buf;
}

fb_line* fb_create_line() {
  fb_line *line = malloc(sizeof(fb_line));
  line->size = INITIAL_GAP_SIZE;
  line->data = malloc(line->size);

  line->gap_start = 0;
  line->gap_end = line->size;

  line->next = NULL;
  line->prev = NULL;
  return line;
}

void fb_insert_line(fb_buffer *buf, fb_line *after, const char *text) {
  fb_line *new_line = fb_create_line();
  int text_len = text ? strlen(text) : 0;

  while (new_line->size <= text_len) {
    new_line->size *= 2;
    new_line->data = realloc(new_line->data, new_line->size);
  }

  if (text_len > 0) {
    memcpy(new_line->data, text, text_len);
  }

  new_line->gap_start = text_len;
  new_line->gap_end = new_line->size;

  if (after == NULL) {
    new_line->next = buf->head;
    if (buf->head) buf->head->prev = new_line;
    buf->head = new_line;
    if (buf->tail == NULL) buf->tail = new_line;
  } else {
    new_line->next = after->next;
    new_line->prev = after;
    if (after->next) after->next->prev = new_line;
    after->next = new_line;
    if (after == buf->tail) buf->tail = new_line;
  }

  buf->total_lines++;
}

void fb_move_gap(fb_line *line, int pos) {
  int current_len = line->gap_start + (line->size - line->gap_end);
  if (pos < 0) pos = 0;
  if (pos > current_len) pos = current_len;

  while (line->gap_start > pos) {
    line->gap_start--;
    line->gap_end--;
    line->data[line->gap_end] = line->data[line->gap_start];
  }

  while (line->gap_start < pos) {
    line->data[line->gap_start] = line->data[line->gap_end];
    line->gap_start++;
    line->gap_end++;
  }
}

void fb_insert_char(fb_line *line, char c) {
  if (line->gap_start == line->gap_end) {
    int old_size = line->size;
    int new_size = old_size * 2;
    line->data = realloc(line->data, new_size);

    int post_gap_len = old_size - line->gap_end;
    int new_gap_end = new_size - post_gap_len;

    memmove(line->data + new_gap_end,
        line->data + line->gap_end,
        post_gap_len);

    line->gap_end = new_gap_end;
    line->size = new_size;
  }

  line->data[line->gap_start] = c;
  line->gap_start++;
}

void fb_split_line(fb_buffer *buf, fb_line *current_line) {
  fb_line *new_line = fb_create_line();
  int post_gap_len = current_line->size - current_line->gap_end;

  if (post_gap_len > 0) {
    while (new_line->size <= post_gap_len) {
      new_line->size *= 2;
      new_line->data = realloc(new_line->data, new_line->size);
      new_line->gap_end = new_line->size;
    }

    memcpy(new_line->data, current_line->data + current_line->gap_end, post_gap_len);
    new_line->gap_start = post_gap_len;
  }

  current_line->gap_end = current_line->size;

  new_line->next = current_line->next;
  new_line->prev = current_line;

  if (current_line->next) {
    current_line->next->prev = new_line;
  } else {
    buf->tail = new_line;
  }
  current_line->next = new_line;
  buf->total_lines++;
}

void fb_free(fb_buffer *buf) {
  if (!buf) return;

  fb_line *line = buf->head;
  while (line) {
    fb_line *next = line->next;
    free(line->data);
    free(line);
    line = next;
  }

  free(buf);
}
