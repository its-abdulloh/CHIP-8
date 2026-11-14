#include "../include/instructions.h"
#include "../include/chip8.h"
#include <stdbool.h>
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

void chip8_8XY3(Chip8 *chip8, uint8_t x, uint8_t y) {
    chip8->V[x] ^= chip8->V[y];
}

void chip8_8XY4(Chip8 *chip8, uint8_t x, uint8_t y){
    uint16_t sum = chip8->V[x]+chip8->V[y];
    if (sum>255){
        chip8->V[0xF]=1;
    }
    else {
        chip8->V[0xF]=0;
    }
    chip8->V[x] = sum& 0xFF;
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
    (void)y;
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
    (void)y;
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
    chip8->I = 0x50 + chip8->V[x] * 5;
}

void chip8_FX33(Chip8 *chip8, uint8_t x){
    int number = chip8->V[x];
    int ones = number%10;
    int tens = (number/10)%10;
    int hundreds = (number/100)%10;

    chip8->memory[chip8->I] = hundreds;
    chip8->memory[chip8->I+1] = tens;
    chip8->memory[chip8->I+2] = ones;
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
    int x_cord = chip8->V[x];
    int y_cord = chip8->V[y];
    chip8->V[0xF]=0;
    if(n!=0){
        for (int i=0;i<n;i++){
            uint8_t sprite = chip8->memory[chip8->I+i];

            for (int bit=0;bit<8;bit++){
                int pixel = (sprite >> (7 - bit)) & 1;

                if (pixel == 1) {
                    int index;
                    if(chip8->high_res==0){
                        int px = ((x_cord + bit) % 64)*2;
                        int py = ((y_cord + i) % 32)*2;
                        index = py * 128 + px;
                        if (chip8->video[index] == 1 ||
                            chip8->video[index + 1] == 1 ||
                            chip8->video[index + 128] == 1 ||
                            chip8->video[index + 129] == 1) {
                            chip8->V[0xF] = 1;
                        }
                        chip8->video[index]^=1;
                        chip8->video[index+1]^=1;
                        chip8->video[index+128]^=1;
                        chip8->video[index+129]^=1;
                    }
                    else if (chip8->high_res==1) {
                        int px = (x_cord + bit) % 128;
                        int py = (y_cord + i) % 64;
                        index = py * 128 + px;
                        if (chip8->video[index] == 1) {
                            chip8->V[0xF] = 1;
                        }
                        chip8->video[index] ^= 1;
                    }
                }
            }
        }
    }
    else if (n==0) {
        for (int i=0;i<16;i++){
            uint16_t sprite = (chip8->memory[chip8->I+i*2]<<8 | chip8->memory[chip8->I+i*2+1]);

            for (int bit=0;bit<16;bit++){
                int pixel = (sprite >> (15 - bit)) & 1;

                if (pixel == 1) {
                    int index;
                    if(chip8->high_res==0){
                        int px = ((x_cord + bit) % 64)*2;
                        int py = ((y_cord + i) % 32)*2;
                        index = py * 128 + px;
                        if (chip8->video[index] == 1 ||
                            chip8->video[index + 1] == 1 ||
                            chip8->video[index + 128] == 1 ||
                            chip8->video[index + 129] == 1) {
                            chip8->V[0xF] = 1;
                        }
                        chip8->video[index]^=1;
                        chip8->video[index+1]^=1;
                        chip8->video[index+128]^=1;
                        chip8->video[index+129]^=1;
                    }
                    else if (chip8->high_res==1) {
                        int px = (x_cord + bit) % 128;
                        int py = (y_cord + i) % 64;
                        index = py * 128 + px;
                        if (chip8->video[index] == 1) {
                            chip8->V[0xF] = 1;
                        }
                        chip8->video[index] ^= 1;
                    }
                }
            }
        }
    }
    chip8->draw_flag = 1;
}

//SUPER-CHIP instructions

void superchip_00FF(Chip8 *chip8){
    chip8->high_res = 1;
    memset(chip8->video, 0, sizeof(chip8->video));   
}

void superchip_00FE(Chip8 *chip8){
    chip8->high_res = 0;
    memset(chip8->video, 0, sizeof(chip8->video));
}

void superchip_00CN(Chip8 *chip8, uint8_t n){
    for (int i = WIDTH*HEIGHT-1;i>=n*WIDTH;i--){
        chip8->video[i] = chip8->video[i-n*WIDTH];
    }
    for (int i=0;i<n*WIDTH;i++){
        chip8->video[i] = 0;
    }
}

void superchip_00FB(Chip8 *chip8){
    int scroll = chip8->high_res ? 4 : 8;
    for (int y=HEIGHT-1; y>=0; y--) {
        for (int x=WIDTH-1; x>=scroll; x--) {
            chip8->video[y*WIDTH+x] = chip8->video[y*WIDTH+x-scroll];
        }
    }

    for (int y=0; y<HEIGHT; y++) {
        for (int x=0; x<scroll; x++) {
            chip8->video[y*WIDTH+x] = 0;
        }
    }

    chip8->draw_flag = 1;
}

void superchip_00FC(Chip8 *chip8){
    int scroll = chip8->high_res ? 4 : 8;
    for (int y=0; y<HEIGHT; y++) {
        for (int x=0; x<WIDTH-scroll; x++) {
            chip8->video[y*WIDTH+x] = chip8->video[y*WIDTH+x+scroll];
        }
    }

    for (int y=0; y<HEIGHT; y++) {
        for (int x=WIDTH-scroll; x<WIDTH; x++) {
            chip8->video[y*WIDTH+x] = 0;
        }
    }

    chip8->draw_flag = 1;
}

void superchip_FX30(Chip8 *chip8, uint8_t x){
    chip8->I = 0xA0 + chip8->V[x]*10;
}

void superchip_FX75(Chip8 *chip8, uint8_t x){
    for (int i=0;i<=x;i++){
        chip8->rpl_flags[i] = chip8->V[i];
    }
    save_rpl(chip8);
}

void superchip_FX85(Chip8 *chip8, uint8_t x){
    for (int i=0;i<=x;i++){
        chip8->V[i] = chip8->rpl_flags[i];
    }
}

//XO-CHIP instructions
void xochip_5XY2(Chip8 *chip8, uint8_t x, uint8_t y){
    for (int i=x;i<=y;i++){
        chip8->memory[chip8->I+(i-x)] = chip8->V[i];
    }
}

void xochip_5XY3(Chip8 *chip8, uint8_t x, uint8_t y){
    for(int i=x;i<=y;i++){
        chip8->V[i] = chip8->memory[chip8->I+(i-x)];
    }
}

void xochip_F000_NNNN(Chip8 *chip8, uint16_t nnnn){
    chip8->I = nnnn;
}

void xochip_FN01(Chip8 *chip8, uint8_t N){
    chip8->plane = N;
}

void xochip_F002(Chip8 *chip8){
    for (int i=0; i<16;i++) {
        chip8->audio_buffer[i] = chip8->memory[chip8->I+i];
    }
}

void xochip_FX3A(Chip8 *chip8, uint8_t x){
    chip8->pitch = chip8->V[x];
}

