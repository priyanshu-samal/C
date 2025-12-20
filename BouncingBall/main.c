#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>

#undef main

int main(void)
{
    SDL_Init(SDL_INIT_VIDEO);

    const int WIDTH  = 800;
    const int HEIGHT = 600;

    SDL_Window *window = SDL_CreateWindow(
        "Red Ball with Orange Trail",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED
    );

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    
    float x = 200.0f;
    float y = 120.0f;
    float vx = 16.0f;     
    float vy = -14.0f;
    float gravity = 0.75f;

    int radius = 24;

    int running = 1;
    SDL_Event event;

    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;
        }

        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 14);
        SDL_RenderFillRect(renderer, NULL);

        
        x += vx;
        y += vy;
        vy += gravity;

        
        if (y + radius >= HEIGHT) {
            y = HEIGHT - radius;
            vy = -vy * 0.96f;   
        }

        
        if (x - radius <= 0 || x + radius >= WIDTH) {
            vx = -vx;
        }

        
        SDL_SetRenderDrawColor(renderer, 255, 140, 0, 80);
        for (int dy = -radius; dy <= radius; dy++) {
            for (int dx = -radius; dx <= radius; dx++) {
                if (dx*dx + dy*dy <= radius*radius) {
                    SDL_RenderDrawPoint(
                        renderer,
                        (int)(x + dx),
                        (int)(y + dy)
                    );
                }
            }
        }

        
        int core = radius - 6;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for (int dy = -core; dy <= core; dy++) {
            for (int dx = -core; dx <= core; dx++) {
                if (dx*dx + dy*dy <= core*core) {
                    SDL_RenderDrawPoint(
                        renderer,
                        (int)(x + dx),
                        (int)(y + dy)
                    );
                }
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); 
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
