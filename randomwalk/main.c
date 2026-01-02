#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define WIDTH  900
#define HEIGHT 800

typedef struct {
    int x;
    int y;
    Uint8 r, g, b;
} Agent;

int main(int argc, char *argv[]) {

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "Random Walk - Fade",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    int agent_count = (argc > 1) ? atoi(argv[1]) : 2000;
    Agent *agents = malloc(sizeof(Agent) * agent_count);

    srand((unsigned int)time(NULL));

    for (int i = 0; i < agent_count; i++) {
        agents[i].x = WIDTH / 2;
        agents[i].y = HEIGHT / 2;

        float t = (float)i / agent_count;
        agents[i].r = (Uint8)(255 * t);
        agents[i].g = (Uint8)(255 * (1.0f - t));
        agents[i].b = (Uint8)(128 + 127 * sin(t * 6.28f));
    }

    int running = 1;
    SDL_Event e;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    while (running) {

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = 0;
        }

        //fade effect by drawing a semi-transparent black rectangle
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 25);
        SDL_Rect fade = { 0, 0, WIDTH, HEIGHT };
        SDL_RenderFillRect(renderer, &fade);

        //update and draw agents
        for (int i = 0; i < agent_count; i++) {
            int dir = rand() % 4;

            if (dir == 0) agents[i].x += 2;
            if (dir == 1) agents[i].x -= 2;
            if (dir == 2) agents[i].y += 2;
            if (dir == 3) agents[i].y -= 2;

            if (agents[i].x < 0) agents[i].x = 0;
            if (agents[i].x >= WIDTH) agents[i].x = WIDTH - 1;
            if (agents[i].y < 0) agents[i].y = 0;
            if (agents[i].y >= HEIGHT) agents[i].y = HEIGHT - 1;

            SDL_SetRenderDrawColor(renderer,
                agents[i].r, agents[i].g, agents[i].b, 255);

            SDL_Rect r = { agents[i].x, agents[i].y, 2, 2 };
            SDL_RenderFillRect(renderer, &r);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    free(agents);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
