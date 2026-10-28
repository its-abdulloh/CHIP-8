#include "instructions.h"
#include "chip8.h"
#include <stdint.h>
#include <string.h>

uint8_t randomByte(){
    uint8_t random_num = rand() % (255 - 0 + 1) + 0;
    return random_num;
}

void chip8_00E0(Chip8 *chip8){
    memset(chip8->video, 0, sizeof(chip8->video));
}

void chip8_00EE(Chip8 *chip8){
    chip8->pc=chip8->stack[chip8->sp];
    chip8->sp-=1;
}

void chip8_1NNN(Chip8 *chip8,uint16_t nnn){
    chip8->pc = nnn;
}

void chip8_2NNN(Chip8 *chip8, uint16_t nnn){
    chip8->sp+=1;
    chip8->stack[chip8->sp]=chip8->pc;
    chip8->pc=nnn;
}

void chip8_3XKK(Chip8 *chip8, uint8_t x, uint8_t kk){
    if (chip8->V[x]==kk) {
        chip8->pc+=2;
    }
}

void chip8_4XKK(Chip8 *chip8, uint8_t x, uint8_t kk){
    if (chip8->V[x]!=kk) {
        chip8->pc+=2;
    }
}

void chip8_5XY0(Chip8 *chip8, uint8_t x, uint8_t y){
    if (chip8->V[x]==chip8->V[y]) {
        chip8->pc+=2;
    }
}

void chip8_6XKK(Chip8 *chip8, uint8_t x, uint8_t kk){
    chip8->V[x] = kk;
}

void chip8_7XKK(Chip8 *chip8,uint8_t x,uint8_t kk){
    chip8->V[x]+=kk;
}

void chip8_8XY0(Chip8 *chip8, uint8_t x, uint8_t y){
    chip8->V[x]=chip8->V[y];
}

void chip8_8XY1(Chip8 *chip8, uint8_t x, uint8_t y){
    chip8->V[x]|=chip8->V[y];
}

void chip8_8XY2(Chip8 *chip8, uint8_t x, uint8_t y){
    chip8->V[x]&=chip8->V[y];
}

void chip8_8XY3(Chip8 *chip8, uint8_t x, uint8_t y){
    if (chip8->V[x]!=chip8->V[y]) {
        chip8->V[x]=1;
    }
    else {
        chip8->V[x]=0;
    }
}

void chip8_8XY4(Chip8 *chip8, uint8_t x, uint8_t y){
    chip8->V[x]+=chip8->V[y];
    if (chip8->V[x]>255){
        chip8->V[0xF]=1;
    }
    else {
        chip8->V[0xF]=0;
    }
}

void chip8_8XY5(Chip8 *chip8, uint8_t x, uint8_t y){
    if (chip8->V[x]>chip8->V[y]){
        chip8->V[0xF]=1;
    }
    else {
        chip8->V[0xF] = 0;
    }
    chip8->V[x]= chip8->V[x]-chip8->V[y];
}

void chip8_8XY6(Chip8 *chip8, uint8_t x, uint8_t y){
    uint8_t lsb = chip8->V[x] & 1;
    chip8->V[x] = chip8->V[x]>>1;
    chip8->V[0xF] = lsb;
}

void chip8_8XY7(Chip8 *chip8, uint8_t x, uint8_t y){
    if (chip8->V[y]>chip8->V[x]){
        chip8->V[0xF]=1;
    }
    else {
        chip8->V[0xF] = 0;
    }
    chip8->V[x] = chip8->V[y]-chip8->V[x];
}

void chip8_8XYE(Chip8 *chip8, uint8_t x, uint8_t y){
    uint8_t msb = (chip8->V[x] & 0x80) >> 7;
    chip8->V[x] = chip8->V[x]<<1;
    chip8->V[0xF] = msb;   
}

void chip8_9XY0(Chip8 *chip8, uint8_t x, uint8_t y){
    if (chip8->V[x]!=chip8->V[y]) {
        chip8->pc+=2;
    }
}

void chip8_ANNN(Chip8 *chip8,uint16_t nnn){
    chip8->I = nnn;
}

void chip8_BNNN(Chip8 *chip8, uint16_t nnn){
    chip8->pc = nnn+chip8->V[0];
}

void chip8_CXKK(Chip8 *chip8, uint8_t x, uint8_t kk){
    chip8->V[x] = randomByte() & kk;
}

void chip8_EX9E(Chip8 *chip8, uint8_t x){
    if (chip8->keypad[chip8->V[x]]==1) {
        chip8->pc+=2;
    }
}

void chip8_EXA1(Chip8 *chip8, uint8_t x){
    if (chip8->keypad[chip8->V[x]]==0) {
        chip8->pc+=2;
    }
}

void chip8_FX07(Chip8 *chip8, uint8_t x){
    chip8->V[x] = chip8->delayTimer;
}

void chip8_FX15(Chip8 *chip8, uint8_t x){
    chip8->delayTimer = chip8->V[x];
}

void chip8_FX18(Chip8 *chip8, uint8_t x){
    chip8->soundTimer = chip8->V[x];
}

void chip8_FX1E(Chip8 *chip8, uint8_t x){
    chip8->I+=chip8->V[x];
}

void chip8_FX0A(Chip8 *chip8, uint8_t x){
    int keypressed = -1;
    for (int i=0;i<16;i++){
        if (chip8->keypad[i]==1){
            keypressed = i;
            break;
        }
    }
    if(keypressed==-1){
        chip8->pc -= 2;
    }
    else {
        chip8->V[x]=keypressed;
    }
}

void chip8_FX29(Chip8 *chip8, uint8_t x){
    chip8->I = chip8->V[x] * 5;
}

void chip8_FX33(Chip8 *chip8, uint8_t x){
    int number = chip8->V[x];
    int ones = number%10;
    int tens = (number/10)%10;
    int hundreds = (number/100)%10;

    chip8->V[chip8->I] = hundreds;
    chip8->V[chip8->I+1] = tens;
    chip8->V[chip8->I+2] = ones;
}

void chip8_FX55(Chip8 *chip8, uint8_t x){
    for (int i=0;i<=x;i++){
        chip8->memory[chip8->I+i] = chip8->V[i];
    }
}

void chip8_FX65(Chip8 *chip8, uint8_t x){
    for (int i=0;i<=x;i++){
        chip8->V[i] = chip8->memory[chip8->I+i];
    }
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

