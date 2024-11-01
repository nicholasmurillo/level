#include "LIS3DH.h"
#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

// I2C address for the LIS3DH (usually 0x18 or 0x19)
const int ADDRESS = 0x18;

// Register addresses
const uint8_t CTRL_REG_1 = 0x20;
const uint8_t CTRL_REG_4 = 0x23;
const uint8_t REG_OUT_X_L = 0x28;
const uint8_t REG_OUT_Y_L = 0x2A;
const uint8_t REG_OUT_Z_L = 0x2C;
const uint8_t REG_OUT_X_H = 0x29;
const uint8_t REG_OUT_Y_H = 0x2B;
const uint8_t REG_OUT_Z_H = 0x2D;
const uint8_t WHO_AM_I = 0x0F;

// Initializes the accelerometer
bool LIS3DH_init(void) {
    uint8_t buf[2];

    // Turn normal mode and 1.344kHz data rate on
    buf[0] = CTRL_REG_1;
    buf[1] = 0x97;
    i2c_write_blocking(i2c_default, ADDRESS, buf, 2, false);

    uint8_t data;
    i2c_write_blocking(i2c_default, ADDRESS, &CTRL_REG_1, 1, true);
    i2c_read_blocking(i2c_default, ADDRESS, &data, 1, false);
    if (data == 0x97) {
        return true;
    } else {
        return false;
    }
}

// Sets a register to a specified value
void LIS3DH_set_reg(uint8_t reg, uint8_t val) {
    uint8_t buf[2];

    // Turn normal mode and 1.344kHz data rate on
    buf[0] = CTRL_REG_1;
    buf[1] = 0x97;
    i2c_write_blocking(i2c_default, ADDRESS, buf, 2, false);
}

// Reads and returns the byte at a specified register
uint8_t LIS3DH_read_reg(uint8_t reg) {
    // Read two bytes of data and store in a 16 bit data structure
    uint8_t data;
    i2c_write_blocking(i2c_default, ADDRESS, &reg, 1, true);
    i2c_read_blocking(i2c_default, ADDRESS, &data, 1, false);
    return data;
}

float LIS3DH_read_data(uint8_t reg) {
    // Read two bytes of data and store in a 16 bit data structure
    uint8_t lsb;
    uint8_t msb;
    uint16_t raw_accel;
    i2c_write_blocking(i2c_default, ADDRESS, &reg, 1, true);
    i2c_read_blocking(i2c_default, ADDRESS, &lsb, 1, false);

    reg |= 0x01;
    i2c_write_blocking(i2c_default, ADDRESS, &reg, 1, true);
    i2c_read_blocking(i2c_default, ADDRESS, &msb, 1, false);

    raw_accel = (msb << 8) | lsb;
    float scaling = 64 / 0.004f;
    float final_value = (float) ((int16_t) raw_accel) / scaling;
    return final_value;
}

// Updates the x, y, and z acceleration values
void LIS3DH_update(float* x, float* y) {
    *x = LIS3DH_read_data(REG_OUT_X_L);
    *y = LIS3DH_read_data(REG_OUT_Y_L);
}
