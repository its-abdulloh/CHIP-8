#include "display.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

SDL_Window* window;
SDL_Renderer* renderer;

const int WIDTH = 64;
const int HEIGHT = 32;
const int SCALE = 10;

int display_init(){
    SDL_Init(SDL_INIT_VIDEO);
    
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

void draw(uint32_t video[]) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int y=0;y<HEIGHT;y++){
        for (int x =0; x<WIDTH; x++) {
            int index = y*WIDTH+x;

            if (video[index]==1) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            }

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
