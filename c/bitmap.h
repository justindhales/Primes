#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BITMAP_ALIGNMENT 64
#define TRUE 1
#define FALSE 0

typedef struct {
  size_t size;
  uint8_t* bytes;
} bitmap_t;

bitmap_t* bitmap_new(size_t size);
void bitmap_init(bitmap_t* bitmap, uint8_t byte);
void bitmap_free(bitmap_t* bitmap);

inline uint8_t bitmap_get(bitmap_t* bitmap, size_t index);
inline int bitmap_get_secure(bitmap_t* bitmap, size_t index, uint8_t* value);

inline void bitmap_set(bitmap_t* bitmap, size_t index, uint8_t value);
inline int bitmap_set_secure(bitmap_t* bitmap, size_t index, uint8_t value);

#endif  // BITMAP_H