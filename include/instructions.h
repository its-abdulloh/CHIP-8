#pragma once
#include "chip8.h"
#include <stdint.h>
#include <stdlib.h>


uint8_t randomByte();
void chip8_00E0(Chip8 *chip8);
void chip8_00EE(Chip8 *chip8);
void chip8_1NNN(Chip8 *chip8,uint16_t nnn);
void chip8_2NNN(Chip8 *chip8,uint16_t nnn);
void chip8_3XKK(Chip8 *chip8,uint8_t x,uint8_t kk);
void chip8_4XKK(Chip8 *chip8,uint8_t x,uint8_t kk);
void chip8_5XY0(Chip8 *chip8,uint8_t x,uint8_t y);
void chip8_6XKK(Chip8 *chip8,uint8_t x,uint8_t kk);
void chip8_7XKK(Chip8 *chip8,uint8_t x,uint8_t kk);
void chip8_8XY0(Chip8 *chip8,uint8_t x, uint8_t y);
void chip8_8XY1(Chip8 *chip8,uint8_t x, uint8_t y);
void chip8_8XY2(Chip8 *chip8,uint8_t x, uint8_t y);
void chip8_8XY3(Chip8 *chip8,uint8_t x, uint8_t y);
void chip8_8XY4(Chip8 *chip8,uint8_t x, uint8_t y);
void chip8_8XY5(Chip8 *chip8,uint8_t x, uint8_t y);
void chip8_8XY6(Chip8 *chip8,uint8_t x, uint8_t y);
void chip8_8XY7(Chip8 *chip8,uint8_t x, uint8_t y);
void chip8_8XYE(Chip8 *chip8,uint8_t x, uint8_t y);
void chip8_9XY0(Chip8 *chip8,uint8_t x,uint8_t y);
void chip8_ANNN(Chip8 *chip8,uint16_t nnn);
void chip8_BNNN(Chip8 *chip8, uint16_t nnn);
void chip8_CXKK(Chip8 *chip8,uint8_t x,uint8_t kk);
void chip8_EX9E(Chip8 *chip8, uint8_t x);
void chip8_EXA1(Chip8 *chip8, uint8_t x);
void chip8_DXYN(Chip8 *chip8,uint8_t x,uint8_t y,uint8_t n);