#include <stdlib.h>
#include <stdio.h>
#include "array_list.h"

MAKE_LIST_FUN(int, int)
MAKE_LIST_FUN(int_pointer, int *)

static inline bool int_equality(const void * const x, const void * const y);
static inline bool int_equality(const void * const x, const void * const y) {
  return (int)x == (int)y;
}

static inline bool int_pointer_equality(const void * const x, const void * const y);
static inline bool int_pointer_equality(const void * const x, const void * const y) {
  return (x == NULL && y == NULL) || *(int *)x == *(int *)y;
}

int main() {
  ArrayList int_list = alloc_int_list();

  add(&int_list, (void *)42);
  add(&int_list, (void *)43);

  int const n = 100;
  int * xs = malloc(n * sizeof *xs);
  for (int i = 0; i < n; i++)
    xs[i] = i+1;

  add_all(&int_list, xs, n);

  int a = (int)(int *)get(&int_list, 0);
  int b = (int)(int *)get(&int_list, 1);
  printf("%d, %d\n", a, b);

  for (size_t i = 0; i < int_list.size; i++)
    printf("%d ", ((int *)int_list.data)[i]);
  puts("");

  remove_at(&int_list, 3);
  
  size_t ix;
  bool found = index_of(&int_list, (void *)3, &ix, int_equality);
  
  if (found)
    printf("Found %d at index %lu\n", ((int *)int_list.data)[ix], ix);
  else
    puts("Not found");

  puts(contains(&int_list, (void *)56, int_equality) ?
       "list contains 56" : "list does not containt 56");

  int * int_out = malloc(int_list.size * sizeof *int_out);
  to_array(&int_list, int_out);

  for (size_t i = 0; i < int_list.size; i++)
    printf("%d ", int_out[i]);
  puts("");

  free(int_list.data);
  free(xs);
  free(int_out);

  ArrayList int_pointer_list = alloc_int_pointer_list();

  int * const restrict x = malloc(sizeof *x), * const restrict y = malloc(sizeof *y);
  *x = 7;
  *y = 8;

  add(&int_pointer_list, x);
  add(&int_pointer_list, y);

  int * px = get(&int_pointer_list, 0);
  int * py = get(&int_pointer_list, 1);
  printf("%d, %d\n", *px, *py);

  puts(contains(&int_pointer_list, x, int_pointer_equality) ?
       "list contains x" : "list does not containt x");
  
  free(x);
  free(y);
  free(int_pointer_list.data);

  return 0;
}
