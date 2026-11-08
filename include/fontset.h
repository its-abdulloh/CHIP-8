#pragma once
#include <stdint.h>

#define FONTSET_SIZE 80
#define BIG_FONTSIZE 160
#define FONTSET_START_ADDRESS 0x50
#define FONT_BIG_START_ADDRESS 0xA0

extern uint8_t fontset[FONTSET_SIZE];

extern uint8_t big_fontset[BIG_FONTSIZE];