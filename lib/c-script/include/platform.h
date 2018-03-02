#ifndef PLATFORM_H
#define PLATFORM_H

typedef unsigned long size_t;
typedef _Bool bool;

#include <stdarg.h>

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifdef QUIET
#define infof(fmt, ...)
#else
__attribute__((format(printf, 1, 2))) void infof(const char * fmt, ...);
#endif
__attribute__((format(printf, 1, 2))) void warnf(const char * fmt, ...);
__attribute__((format(printf, 1, 2))) void errorf(const char * fmt, ...);

__attribute__((malloc, returns_nonnull, alloc_size(1))) void * mem_allocate(size_t size);
__attribute__((nonnull)) void mem_free(void * ptr);

__attribute__((nonnull)) void * mem_move(void * dest, const void * src, size_t n);

__attribute__((nonnull)) int str_compare(const char * str1, const char * str2);

#endif /* PLATFORM_H */
