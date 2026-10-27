#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "include/fontset.h"
#include "include/instructions.h"
#include "include/chip8.h"
#include "include/display.h"


//memory map
// 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
// 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
// 0x200-0xFFF - Program ROM and work RAM
#define ROM_START 0x200

Chip8 chip8;


int map_key(SDL_Keycode key)
{
    switch (key) {
        case SDL_SCANCODE_1: return 0x1;
        case SDL_SCANCODE_2: return 0x2;
        case SDL_SCANCODE_3: return 0x3;
        case SDL_SCANCODE_4: return 0xC;

        case SDL_SCANCODE_Q: return 0x4;
        case SDL_SCANCODE_W: return 0x5;
        case SDL_SCANCODE_E: return 0x6;
        case SDL_SCANCODE_R: return 0xD;

        case SDL_SCANCODE_A: return 0x7;
        case SDL_SCANCODE_S: return 0x8;
        case SDL_SCANCODE_D: return 0x9;
        case SDL_SCANCODE_F: return 0xE;

        case SDL_SCANCODE_Z: return 0xA;
        case SDL_SCANCODE_X: return 0x0;
        case SDL_SCANCODE_C: return 0xB;
        case SDL_SCANCODE_V: return 0xF;

        default: return -1;
    }
}

void initialize(){
    chip8 = (Chip8){0};

    srand((unsigned int)time(NULL));
    for (unsigned int i=0;i<FONTSET_SIZE;i++){
        chip8.memory[FONTSET_START_ADDRESS+i] = fontset[i];
    }
    display_init();
    return;
}

int LoadROM(char const *filename){
    FILE *file = fopen(filename,"rb");

    if (file==NULL){
        fprintf(stderr,"Failed to open ROM\n");
        return 0;
    }

    if (fseek(file, 0, SEEK_END)!=0){
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

    rewind(file);
    size_t bytes_read = fread(&chip8.memory[ROM_START], 1, (size_t) file_size, file);

    fclose(file);

    if (bytes_read!=(size_t)file_size){
        fprintf(stderr,"Failed to read complete ROM\n");
        return 0;
    }
    
    chip8.pc = ROM_START;

    return 1;
}


void fetch(){
    chip8.opcode = chip8.memory[chip8.pc]*256+chip8.memory[chip8.pc+1];
    chip8.pc+=2;
    printf("PC: %03X OPCODE: %04X\n", chip8.pc, chip8.opcode);
}

void decode_exec(){
    uint8_t op = (chip8.opcode & 0xF000)>>12;
    uint8_t x = (chip8.opcode & 0x0F00)>>8;
    uint8_t y = (chip8.opcode & 0x00F0)>>4;
    uint8_t n = chip8.opcode & 0x000F;
    uint8_t kk = chip8.opcode & 0x00FF;
    uint16_t nnn = chip8.opcode & 0x0FFF;

    switch (op) {
        case 0:
        if (kk==0xE0) {
            chip8_00E0(&chip8);
        }
        else if (kk==0xEE) {
            chip8_00EE(&chip8);
        }
        break;
        case 1:
        chip8_1NNN(&chip8, nnn);
        break;
        case 2:
        chip8_2NNN(&chip8, nnn);
        break;
        case 3:
        chip8_3XKK(&chip8, x, kk);
        break;
        case 4:
        chip8_4XKK(&chip8, x, kk);
        break;
        case 5:
        chip8_5XY0(&chip8, x, y);
        break;
        case 6:
        chip8_6XKK(&chip8, x, kk);
        break;
        case 7:
        chip8_7XKK(&chip8, x, kk);
        break;
        case 8:
        switch (n) {
            case 0:
            chip8_8XY0(&chip8, x, y);
            break;        
            case 1:
            chip8_8XY1(&chip8, x, y);
            break;
            case 2:
            chip8_8XY2(&chip8, x, y);
            break;
            case 3:
            chip8_8XY3(&chip8, x, y);
            break;
            case 4:
            chip8_8XY4(&chip8, x, y);
            break;
            case 5:
            chip8_8XY5(&chip8, x, y);
            break;
            case 6:
            chip8_8XY6(&chip8, x, y);
            break;
            case 7:
            chip8_8XY7(&chip8, x, y);
            break;
            case 0xE:
            chip8_8XYE(&chip8, x, y);
            break;
        }
        break;
        case 9:
        chip8_9XY0(&chip8, x, y);
        break;
        case 0xA:
        chip8_ANNN(&chip8, nnn);
        break;
        case 0xB:
        chip8_BNNN(&chip8,nnn);
        break;
        case 0xC:
        chip8_CXKK(&chip8, x, kk);
        break;
        case 0xE:
        chip8_EX9E(&chip8, x);
        break;
        case 0xD:
        chip8_DXYN(&chip8, x, y, n);
        draw(chip8.video);
        break;
    }
}

int main(){
    initialize();
    if (!LoadROM("IBM Logo.ch8")) {
        fprintf(stderr, "Failed to load ROM. Exiting.\n");
        return EXIT_FAILURE;
    }
    bool running = true;
    SDL_Event event;
    while (running){
        //handle keys
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN && !event.key.repeat) {
                int key = map_key(event.key.keysym.scancode);

                if (key != -1)
                    chip8.keypad[key] = 1;
            }
            if (event.type==SDL_KEYUP) {
                int key = map_key(event.key.keysym.scancode);

                if (key!=-1)
                    chip8.keypad[key]=0;
            }
        }
        fetch();
        decode_exec();
    }
    display_quit();
    return EXIT_SUCCESS;
}