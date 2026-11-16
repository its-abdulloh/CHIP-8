#include "../include/display.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdlib.h>

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;
SDL_PixelFormat *pixelFormat;

const int WIDTH = 128;
const int HEIGHT = 64;
const int SCALE = 5;

SDL_Color color_pallate[] = {
    {  255,   255,   255, 255 }, // Level 0: White
    { 85,  85,  85, 255 }, // Level 1: Dark Gray
    {170, 170, 170, 255 }, // Level 2: Light Gray
    {0, 0, 0, 255 }  // Level 3: Black
};

static uint32_t *pixels = NULL;

int display_init(){
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    
    window = SDL_CreateWindow(
        "chip8",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640, 320,
        SDL_WINDOW_SHOWN
    );

    if (!window) return -1;

    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED
    );

    if (!renderer) return -1;
    
    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH,
        HEIGHT
    );

    pixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    pixels = malloc(WIDTH * HEIGHT * sizeof(uint32_t));

    return 0;
}

void draw(uint8_t video[], uint8_t video1[]) {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        int color_i = video[i] | (video1[i] << 1);
        SDL_Color c = color_pallate[color_i];

        pixels[i] = SDL_MapRGBA(
            pixelFormat, c.r, c.g, c.b, 255
        );
    }

    SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void display_quit(){
    SDL_FreeFormat(pixelFormat);
    free(pixels);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
