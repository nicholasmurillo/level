#include "neomatrix.h"
#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "./build/neopixel.pio.h"

static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}

// Initialize NeoMatrix struct, allocate buffer, and configure PIO
bool NeoMatrix_init(void) {
    // Initialize power to the NeoMatrix LEDs
    gpio_init(10);
    gpio_set_dir(10, GPIO_OUT);
    gpio_put(10, true);

    // Initialize PIO and configure for WS2812 protocol
    PIO pio = pio0;
    int sm = 0;

    // Add the WS2812 program to the PIO memory and get the offset
    uint offset = pio_add_program(pio, &ws2812_program);

    // Initialize the WS2812 program for the claimed state machine
    ws2812_program_init(pio, sm, offset, 7, 800000, false);

    return true;
}

// Set the pixel color at a specific (row, col) location
void NeoMatrix_set_pixel(uint8_t width, uint8_t height, uint32_t pixel_buffer[], uint8_t row, uint8_t col, uint32_t color) {
    if (row >= height || col >= width) {
        return;
    }
    // Calculate the index in the pixel buffer
    uint16_t index = row * width + col;

    // Set the color at the calculated index
    pixel_buffer[index] = color;
}

// Clear all pixels in the buffer
void NeoMatrix_clear_pixels(uint8_t width, uint8_t height, uint32_t pixel_buffer[]) {
    // Set all elements of the pixel buffer to 0 (black)
    for (uint i = 0; i < width * height; ++i)
        pixel_buffer[i] = 0x00000000;
}

// Write the pixel buffer to the NeoMatrix
void NeoMatrix_write(uint8_t width, uint8_t height, uint32_t pixel_buffer[]) {
    for (uint i = 0; i < width * height; ++i) {
        // Extract RGB components from pixel_buffer[i]
        uint8_t red = (pixel_buffer[i] >> 16) & 0xFF;
        uint8_t green = (pixel_buffer[i] >> 8) & 0xFF;
        uint8_t blue = pixel_buffer[i] & 0xFF;

        // Convert to GRB format for WS2812 and send to the LEDs
        uint32_t grb_color = urgb_u32(red, green, blue);
        put_pixel(grb_color);
    }
    return;
}
