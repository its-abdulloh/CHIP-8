#pragma once
#include "chip8.h"
#include <stdint.h>
#include <stdlib.h>


uint8_t randomByte();
void chip8_00E0(Chip8 *chip8);
void chip8_1NNN(Chip8 *chip8,uint16_t nnn);
void chip8_6XKK(Chip8 *chip8,uint8_t x,uint8_t kk);
void chip8_ANNN(Chip8 *chip8,uint16_t nnn);
void chip8_DXYN(Chip8 *chip8,uint8_t x,uint8_t y,uint8_t n);
void chip8_7XKK(Chip8 *chip8,uint8_t x,uint8_t kk);