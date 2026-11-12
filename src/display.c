#include "../include/display.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

SDL_Window* window;
SDL_Renderer* renderer;

const int WIDTH = 128;
const int HEIGHT = 64;
const int SCALE = 5;

SDL_Color color_pallate[] = {
    {  0,   0,   0, 255 }, // Level 0: Black
    { 85,  85,  85, 255 }, // Level 1: Dark Gray
    {170, 170, 170, 255 }, // Level 2: Light Gray
    {255, 255, 255, 255 }  // Level 3: White
};

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

    return 0;
}

void draw(uint8_t video[],uint8_t video1[]) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int y=0;y<HEIGHT;y++){
        for (int x =0; x<WIDTH; x++) {
            int index = y*WIDTH+x;
            
            int color_i = (video[index] << 1) | video1[index];
            SDL_SetRenderDrawColor(
                renderer,
                color_pallate[color_i].r,
                color_pallate[color_i].g,
                color_pallate[color_i].b,
                255
            );

            SDL_Rect rect = {
                x*SCALE,
                y*SCALE,
                SCALE,
                SCALE
            };

            SDL_RenderFillRect(renderer, &rect);
        }
    }

    SDL_RenderPresent(renderer);
}

void display_quit(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
