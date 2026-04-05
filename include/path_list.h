#ifndef PATH_LIST_H
#define PATH_LIST_H

#include <dirent.h>
#include <stdbool.h>

typedef struct {
  char name[256];
  bool is_dir;
} PathEntry;

typedef struct {
  PathEntry* entries;
  int size;
  int capacity;
} path_list;

void path_list_init(path_list *list, int capacity);
void path_list_add(path_list *list, PathEntry entry);
void path_list_free(path_list *list);


#endif
