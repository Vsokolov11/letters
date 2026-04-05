#include "path_list.h"

#include <stdlib.h>
#include <unistd.h>

void path_list_init(path_list* list, int capacity) {
  // maybe make the first element .. to go back :D
  list->entries = malloc(capacity * sizeof(PathEntry));
  list->size = 0;
  list->capacity = capacity;
}

void path_list_free(path_list* list) {
  if (list == NULL) return;

  if (list->entries != NULL) {
    free(list->entries);
    list->entries = NULL;
  }
}

void path_list_add(path_list *list, PathEntry entry) {
  if (list->size == list->capacity) {
    list->capacity *= 2;
    list->entries = realloc(list->entries, list->capacity * sizeof(PathEntry));
  }
  list->entries[list->size++] = entry;
}
