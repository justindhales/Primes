#include "bitmap.h"

bitmap_t* bitmap_new(size_t size) {
  bitmap_t* bitmap = malloc(sizeof(bitmap_t));

  bitmap->size = BITMAP_ALIGNMENT * ((size / BITMAP_ALIGNMENT) + 1);
  bitmap->bytes = aligned_alloc(BITMAP_ALIGNMENT, bitmap->size);
  return bitmap;
}

void bitmap_init(bitmap_t* bitmap, uint8_t byte) {
  memset(bitmap->bytes, byte, bitmap->size);
}

void bitmap_free(bitmap_t* bitmap) {
  free(bitmap->bytes);
  free(bitmap);
}

// uint8_t bitmap_get(bitmap_t* bitmap, size_t index) {
//   return (bitmap->bytes[index / 8] >> (index % 8)) & 1;
// }

// int bitmap_get_secure(bitmap_t* bitmap, size_t index, uint8_t* value) {
//   if (index >= bitmap->size) {
//     return FALSE;
//   }

//   *value = bitmap_get(bitmap, index);
//   return TRUE;
// }

// void bitmap_set(bitmap_t* bitmap, size_t index, uint8_t value) {
//   // Convert value to boolean with (value > 0)
//   if (value > 0) {
//     bitmap->bytes[index / 8] |= (1 << (index % 8));
//   } else {
//     bitmap->bytes[index / 8] &= ~(1 << (index % 8));
//   }
// }

// int bitmap_set_secure(bitmap_t* bitmap, size_t index, uint8_t value) {
//   if (index >= bitmap->size) {
//     return FALSE;
//   }

//   bitmap_set(bitmap, index, value);
//   return TRUE;
// }