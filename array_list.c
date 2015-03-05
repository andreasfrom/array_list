#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "array_list.h"

static size_t next_power_of_two(size_t const v);

void * set(ArrayList * const list, size_t const ix, void * const x) {
  assert(list != NULL);
  assert(ix < list->size);
  void * deleted = list->get(list->data, ix);
  list->set(list->data, ix, x);
  return deleted;
}

void * get(const ArrayList * const list, size_t const ix) {
  assert(list != NULL);
  assert(ix < list->size);
  return list->get(list->data, ix);
}

void add(ArrayList * const list, void * const x) {
  add_at(list, list->size, x);
}

void add_at(ArrayList * const list, size_t const ix, void * const x) {
  assert(list != NULL);
  assert(ix <= list->size);

  if (list->size >= list->capacity) {
    list->capacity *= 2;
    list->data = realloc(list->data, list->capacity * list->element_size);
    assert(list->data != NULL);
  }

  if (ix < list->size)
    memmove(list->offset(list->data, ix+1),
	    list->offset(list->data, ix),
	    (list->size-ix) * list->element_size);

  list->set(list->data, ix, x);
  list->size++;
}

void add_all(ArrayList * const list, void * const xs, size_t const len) {
  add_all_at(list, list->size, xs, len);
}

void add_all_at(ArrayList * const list, size_t const ix, void * const xs, size_t const len) {
  assert(list != NULL);
  assert(xs != NULL);
  assert(ix <= list->size);

  if (list->size + len > list->capacity) {
    list->capacity = next_power_of_two(list->size + len);
    list->data = realloc(list->data, list->capacity * list->element_size);
    assert(list->data != NULL);
  }

  if (ix < list->size)
    memmove(list->offset(list->data, ix+len),
	    list->offset(list->data, ix),
	    (list->size-ix) * list->element_size);

  memcpy(list->offset(list->data, ix), xs, len * list->element_size);
  list->size += len;
}

void * remove_elem(ArrayList * const list, void * const x, EqFun equals) {
  assert(list != NULL);
  assert(equals != NULL);

  size_t ix;
  bool found = index_of(list, x, &ix, equals);

  if (found) {
    void * deleted = list->get(list->data, ix);
    remove_at(list, ix);
    list->size--;
    return deleted;
  }
  
  return NULL;
}

void * remove_at(ArrayList * const list, size_t const ix) {
  assert(list != NULL);
  assert(ix < list->size);

  void * deleted = list->get(list->data, ix);
  
  memmove(list->offset(list->data, ix),
	  list->offset(list->data, ix+1),
	  (list->size-ix) * list->element_size);

  list->size--;

  return deleted;
}

void ensure_capacity(ArrayList * const list, size_t const min_capacity) {
  assert(list != NULL);

  if (list->capacity < min_capacity) {
    list->capacity = next_power_of_two(min_capacity);
    list->data = realloc(list->data, list->capacity * list->element_size);
    assert(list->data != NULL);
  }
}

bool is_empty(const ArrayList * const list) {
  assert(list != NULL);
  return list->size == 0;
}

bool contains(const ArrayList * const list, const void * const x, EqFun equals) {
  assert(list != NULL);
  assert(equals != NULL);
  size_t u;
  return index_of(list, x, &u, equals);
}

bool index_of(const ArrayList * const list, const void * const x, size_t * out, EqFun equals) {
  assert(list != NULL);
  assert(out != NULL);
  assert(equals != NULL);

  for (size_t i = 0; i < list->size; i++) {
    if (equals(x, list->get(list->data, i))) {
      *out = i;
      return true;
    }
  }
  return false;
}

void to_array(const ArrayList * const list, void * const out) {
  assert(list != NULL);
  assert(out != NULL);

  for (size_t i = 0; i < list->size; i++)
    list->set(out, i, list->get(list->data, i));
}

static size_t next_power_of_two(size_t v) {
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v |= v >> 32;
  v++;
  return v;
}
