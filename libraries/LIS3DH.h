#ifndef LIS3DH_H
#define LIS3DH_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    float x, y, z;
} LIS3DH;

// Function prototypes
bool LIS3DH_init(void);
void LIS3DH_set_reg(uint8_t reg, uint8_t val);
uint8_t LIS3DH_read_reg(uint8_t reg);
float LIS3DH_read_data(uint8_t reg);
void LIS3DH_update(float* x, float* y);

#endif // LIS3DH_H
