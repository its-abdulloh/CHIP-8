#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <time.h>


//memory map
// 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
// 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
// 0x200-0xFFF - Program ROM and work RAM
#define MEMORY_SIZE 4096
#define ROM_START 0x200

#define FONTSET_SIZE 80
#define FONTSET_START_ADDRESS 0x50

typedef struct 
{
    //registers
    uint8_t V[16];
    //4096 bytes of memory
    uint8_t memory[MEMORY_SIZE];
    //index register
    uint16_t I;
    //program counter
    uint16_t pc;
    //stack
    uint16_t stack[16];
    //stack pointer
    uint8_t sp;
    //timers
    uint8_t delayTimer;
    uint8_t soundTimer;
    //keypad
    uint8_t keypad[16];
    //graphics
    uint32_t video[64*32];
    //operation code
    uint16_t opcode;
} Chip8;

Chip8 chip8;

uint8_t fontset[FONTSET_SIZE] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

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

int main(){
    initialize();
    LoadROM("test-opcode.ch8");
    return 0;
}