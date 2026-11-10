#include "../include/chip8.h"
#include "stdio.h"
#include <string.h>

void save_rpl(Chip8 *chip8) {
    FILE *file = fopen("chip8_rpl.bin", "wb");

    if (file == NULL) return;

    fwrite(chip8->rpl_flags, 1, 16, file);
    fclose(file);
}

void load_rpl(Chip8 *chip8) {
    FILE *file = fopen("chip8_rpl.bin", "rb");

    if (file == NULL) {
        memset(chip8->rpl_flags, 0, 16);
        return;
    }

    size_t count = fread(chip8->rpl_flags, 1, 16, file);
    fclose(file);

    if (count != 16) {
        memset(chip8->rpl_flags, 0, 16);
    }
}