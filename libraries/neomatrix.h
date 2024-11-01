#ifndef NEOMATRIX_H
#define NEOMATRIX_H

#include <stdint.h>
#include <stdbool.h>

// Function prototypes
static inline void put_pixel(uint32_t pixel_grb);
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);
bool NeoMatrix_init(void);
void NeoMatrix_set_pixel(uint8_t width, uint8_t height, uint32_t pixel_buffer[], uint8_t row, uint8_t col, uint32_t color);
void NeoMatrix_clear_pixels(uint8_t width, uint8_t height, uint32_t pixel_buffer[]);
void NeoMatrix_write(uint8_t width, uint8_t height, uint32_t pixel_buffer[]);

#endif // NEOMATRIX_H