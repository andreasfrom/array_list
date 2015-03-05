#include <stdlib.h>
#include <stdbool.h>

typedef void (* const SetFun)(void * const restrict data, size_t const ix, void * const restrict x);
typedef void * (* const GetFun)(const void * const x, size_t const ix);
typedef void * (* const OffsetFun)(const void * const data, size_t const x);
typedef bool (* const EqFun)(const void * const x, const void * const y);

typedef struct ArrayList ArrayList;

struct ArrayList {
  void * data;
  size_t size;
  size_t capacity;
  size_t const element_size;
  const SetFun set;
  const GetFun get;
  const OffsetFun offset;
};

void * set(ArrayList * const list, size_t const ix, void * const x);
void * get(const ArrayList * const list, size_t const ix);
void add_at(ArrayList * const list, size_t const ix, void * const x);
void add(ArrayList * const list, void * const x);
void add_all(ArrayList * const list, void * const xs, size_t const len);
void add_all_at(ArrayList * const list, size_t const ix, void * const xs, size_t const len);
void * remove_elem(ArrayList * const list, void * const x, EqFun equals);
void * remove_at(ArrayList * const list, size_t const ix);
void ensure_capacity(ArrayList * const list, size_t const min_capacity);
bool index_of(const ArrayList * const, const void * const x, size_t * out, EqFun equals);
bool contains(const ArrayList * const list, const void * const x, EqFun equals);
void to_array(const ArrayList * const list, void * const out);
bool is_empty(const ArrayList * const list);

#define MAKE_SET_FUN(name, type)					\
  static inline void name(void * const restrict data, size_t const ix, void * const restrict x); \
  static inline void name(void * const restrict data, size_t const ix, void * const restrict x) { \
    ((type *) data)[ix] = (type) x;					\
  }
  
#define MAKE_OFFSET_FUN(name, type)					\
  static inline void * name(const void * const data, size_t const ix);	\
  static inline void * name(const void * const data, size_t const ix) {	\
    return (type *)data + ix;						\
  }									\

#define MAKE_GET_FUN(name, type)					\
  static inline void * name(const void * const data, size_t const ix);	\
  static inline void * name(const void * const data, size_t const ix) {	\
    return (void *)(uintptr_t)((type *)data)[ix];			\
  }									\

#define MAKE_LIST_FUN(basename, type)					\
  MAKE_GET_FUN(basename##_get, type)					\
  MAKE_SET_FUN(basename##_set, type)					\
  MAKE_OFFSET_FUN(basename##_offset, type)				\
  ArrayList alloc_##basename##_list(void);				\
  ArrayList alloc_##basename##_list(void) {				\
    void * const data = malloc(8 * sizeof (type));			\
    ArrayList list = {.data = data,					\
		      .size = 0,					\
		      .capacity = 8,					\
		      .element_size = sizeof (type),			\
		      .set = basename##_set,				\
		      .get = basename##_get,				\
		      .offset = basename##_offset};			\
    return list;							\
  }
