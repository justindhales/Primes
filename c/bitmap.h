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

inline uint8_t bitmap_get(bitmap_t* bitmap, size_t index) {
  return (bitmap->bytes[index / 8] >> (index % 8)) & 1;
}

inline int bitmap_get_secure(bitmap_t* bitmap, size_t index, uint8_t* value) {
  if (index >= bitmap->size) {
    return FALSE;
  }

  *value = bitmap_get(bitmap, index);
  return TRUE;
}

inline void bitmap_set(bitmap_t* bitmap, size_t index, uint8_t value) {
  if (value > 0) {
    bitmap->bytes[index / 8] |= (1 << (index % 8));
  } else {
    bitmap->bytes[index / 8] &= ~(1 << (index % 8));
  }
}

inline int bitmap_set_secure(bitmap_t* bitmap, size_t index, uint8_t value) {
  if (index >= bitmap->size) {
    return FALSE;
  }

  bitmap_set(bitmap, index, value);
  return TRUE;
}

#endif  // BITMAP_H