#pragma once

#include <SDL2/SDL.h>
#include <stdint.h>

int display_init();
void draw(uint8_t video[],uint8_t video1[]);
void display_quit();