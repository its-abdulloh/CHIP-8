#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <time.h>
#include <stdbool.h>
#include "include/fontset.h"
#include "include/instructions.h"
#include "include/chip8.h"


//memory map
// 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
// 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
// 0x200-0xFFF - Program ROM and work RAM
#define ROM_START 0x200

Chip8 chip8;

int LoadROM(char const *filename){
    FILE *file = fopen(filename,"rb");

    if (file==NULL){
        fprintf(stderr,"Failed to open ROM\n");
        return 0;
    }

    if (fseek(file, 0, SEEK_END!=0)){
        fclose(file);
        return 0;
    }

    long file_size = ftell(file);

    if (file_size<0){
        fclose(file);
        return 0;
    }

    if (file_size>MEMORY_SIZE - ROM_START) {
        fprintf(stderr,"ROM is too big!\n");
        fclose(file);
        return 0;
    }

    size_t bytes_read = fread(&chip8.memory[ROM_START], 1, (size_t) file_size, file);

    fclose(file);

    if (bytes_read!=(size_t)file_size){
        fprintf(stderr,"Failed to read complete ROM\n");
        return 0;
    }
    
    chip8.pc = ROM_START;

    return 1;
}

void initialize(){
    chip8 = (Chip8){0};
    for (unsigned int i=0;i<FONTSET_SIZE;i++){
        chip8.memory[FONTSET_START_ADDRESS+i] = fontset[i];
    }
    return;
}

uint8_t randomByte(){
    srand(time(NULL));
    uint8_t random_num = rand() % (255 - 0 + 1) + 0;
    return random_num;
}

void fetch(){
    chip8.opcode = chip8.memory[chip8.pc]*256+chip8.memory[chip8.pc+1];
    chip8.pc+=2;
}

void decode(){
    uint8_t op = (chip8.opcode & 0xF000)>>12;
    uint8_t x = (chip8.opcode & 0x0F00)>>8;
    uint8_t y = (chip8.opcode & 0x00F0)>>4;
    uint8_t n = chip8.opcode & 0x000F;
    uint8_t kk = chip8.opcode & 0x00FF;
    uint16_t nnn = chip8.opcode & 0x0FFF;

    switch (op) {
        case 0:
        if (kk==0xE0) {
            //CLS
        }
        else if (kk==0xEE) {
            //RET
        }
        break;
        case 1:
        //JMP to nnn
        break;
        case 6:
        //Set vx to kk
        break;
        case 7:
        //ADD kk to vx
        break;
        case 0xA:
        //Set I to nnn
        break;
        case 0xD:
        //draw
        break;
    }
}

int main(){
    initialize();
    LoadROM("IBM Logo.ch8");
    while (true){

    }
    return 0;
}