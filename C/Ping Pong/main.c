#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Ping pong",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        0
    );

    SDL_Surface *surface = SDL_GetWindowSurface(window);

    SDL_Rect pl1 = {40, 40, 40, 150};
    SDL_Rect pl2 = {720, 40, 40, 150};
    Uint32 color = 0xFFFFFFFF;

    SDL_FillRect(surface, &pl1, color);
    SDL_FillRect(surface, &pl2, color);
    SDL_UpdateWindowSurface(window);

    int running = 1;

    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {
                running = 0;
            }

            if (event.type == SDL_KEYDOWN) {

                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = 0;
                }

                if (event.key.keysym.sym == SDLK_w) {
                    pl1.y -= 10;
                }

                if (event.key.keysym.sym == SDLK_s) {
                    pl1.y += 10;
                }

                if (event.key.keysym.sym == SDLK_UP) {
                    pl2.y -= 10;
                }

                if (event.key.keysym.sym == SDLK_DOWN) {
                    pl2.y += 10;
                }

                SDL_FillRect(surface, NULL, 0x000000FF);
                SDL_FillRect(surface, &pl1, color);
                SDL_FillRect(surface, &pl2, color);
                SDL_UpdateWindowSurface(window);
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
