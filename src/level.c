#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "./libraries/LIS3DH.h"
#include "./libraries/neomatrix.h"

// Define LED matrix dimensions
const uint8_t MATRIX_WIDTH = 8;
const uint8_t MATRIX_HEIGHT = 8;

uint32_t pixel_buffer[64];

// Create LIS3DH variables
float accel_x = 0.0f;
float accel_y = 0.0f;

// Threshold values for determining level
const float LEVEL_THRESHOLD = 0.1f; // ±0.1g
void setup() {
    // Initialize matrix
    if (!NeoMatrix_init()) {
        while (true) {
            printf("Matrix initialization failed!\n");
            sleep_ms(1000);
        }
    }
    // Initialize LIS3DH accelerometer
    if (!LIS3DH_init()) {
        while (true) {
            printf("LIS3DH initialization failed!\n");
            sleep_ms(1000);
        }
    }
}

void loop() {
    LIS3DH_update(&accel_x, &accel_y);
    printf("X acceleration: %.3fg\n", accel_x);
    printf("Y acceleration: %.3fg\n", accel_y);

    // Clear the matrix pixels before setting new ones
    NeoMatrix_clear_pixels(MATRIX_WIDTH, MATRIX_HEIGHT, pixel_buffer);
    
    // Check if +/- 1g exceeded
    if (fabs(accel_x) > 1) {
        if(accel_x > 0) {
            accel_x = 1;
        } else {
            accel_x = -1;
        }
    }
    if (fabs(accel_y) > 1) {
        if(accel_y > 0) {
            accel_y = 1;
        } else {
            accel_y = -1;
        }
    }
    // Check if the device is level
    if (accel_x >= -LEVEL_THRESHOLD && accel_x <= LEVEL_THRESHOLD && accel_y >= -LEVEL_THRESHOLD && accel_y <= LEVEL_THRESHOLD) {
        // Device is level; light up the center 4 LEDs in green
        NeoMatrix_set_pixel(MATRIX_WIDTH, MATRIX_HEIGHT, pixel_buffer, 3, 3, 0x00FF00); // Green
        NeoMatrix_set_pixel(MATRIX_WIDTH, MATRIX_HEIGHT, pixel_buffer, 3, 4, 0x00FF00); // Green
        NeoMatrix_set_pixel(MATRIX_WIDTH, MATRIX_HEIGHT, pixel_buffer, 4, 3, 0x00FF00); // Green
        NeoMatrix_set_pixel(MATRIX_WIDTH, MATRIX_HEIGHT, pixel_buffer, 4, 4, 0x00FF00); // Green
        printf("Level!\n");
    } else {
        // Scale accel_x and accel_y to matrix coordinates (0 to 7) with a maximum at ±0.7g
        uint8_t col = (uint8_t)(5 * (1 - accel_x / 0.7)); // Flips x-axis and scales to ±0.7g range
        uint8_t row = (uint8_t)(5 * (1 - accel_y / 0.7)); // Scales y-axis to ±0.7g range

        // Clamp row and col within matrix bounds
        if (row >= MATRIX_HEIGHT) row = MATRIX_HEIGHT - 1;
        if (col >= MATRIX_WIDTH) col = MATRIX_WIDTH - 1;
        // Light up the calculated LED position in red
        NeoMatrix_set_pixel(MATRIX_WIDTH, MATRIX_HEIGHT, pixel_buffer, row, col, 0xFF0000); // Red
        printf("Tilting!\n");
    }

    // Write the updated pixel buffer to the matrix
    NeoMatrix_write(MATRIX_WIDTH, MATRIX_HEIGHT, pixel_buffer);
    sleep_ms(250);
    // Clear terminal 
    printf("\033[1;1H\033[2J");
}

int main() {
    stdio_init_all();

    // This example will use I2C1 on the default SDA and SCL pins (4, 5 on a Pico)
    i2c_init(i2c1, 400 * 1000); // was 400 * 1000
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C); // was PICO_DEFAULT_I2C_SDA_PIN (2)
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));
    
    // setup();
    setup();

    while (true) {
        loop();
    }

    return 0; // Should never reach here
}
