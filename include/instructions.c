#include "instructions.h"
#include <stdint.h>
#include <string.h>

uint8_t randomByte(){
    uint8_t random_num = rand() % (255 - 0 + 1) + 0;
    return random_num;
}

void chip8_00E0(Chip8 *chip8){
    memset(chip8->video, 0, sizeof(chip8->video));
}

void chip8_1NNN(Chip8 *chip8,uint16_t nnn){
    chip8->pc = nnn;
}

void chip8_6XKK(Chip8 *chip8, uint8_t x, uint8_t kk){
    chip8->V[x] = kk;
}

void chip8_ANNN(Chip8 *chip8,uint16_t nnn){
    chip8->I = nnn;
}

void chip8_DXYN(Chip8 *chip8, uint8_t x, uint8_t y, uint8_t n){
    chip8->V[0xF]=0;
    int x_cord = chip8->V[x];
    int y_cord = chip8->V[y];
    for (int i=0;i<n;i++){
        uint8_t sprite = chip8->memory[chip8->I+i];

        for (int bit=0;bit<8;bit++){
            int pixel = (sprite >> (7 - bit)) & 1;

            if (pixel == 1) {
                int px = (x_cord + bit) % 64;
                int py = (y_cord + i) % 32;
                int index = py * 64 + px;

                if (chip8->video[index] == 1) {
                    chip8->V[0xF] = 1;
                }

                chip8->video[index] ^= 1;
            }
        }
    }
}
