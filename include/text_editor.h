#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include "file_buffer.h"

#define CTRL(c) ((c) & 0x1f)

int open_file_editor();
fb_buffer* load_file_in_buffer(const char *path);
void fb_render_line(fb_line *line, int row);
int save_file(fb_buffer *buf, const char *path);

#endif
