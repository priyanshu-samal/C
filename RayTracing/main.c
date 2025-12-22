#include <SDL2/SDL.h>
#include <stdio.h>

#define WIDTH  1000
#define HEIGHT 800

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Ray Tracing",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_SHOWN
    );

    SDL_Surface *surface = SDL_GetWindowSurface(window);
    if (surface == NULL) {
        printf("SDL_GetWindowSurface Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Rect rect = {200, 200, 500, 500};
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0, 0, 255));
    SDL_UpdateWindowSurface(window);

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
