#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>

int main(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "ASCII Spinner",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    const char *frames[] = {
        "    O    ",
        "   \\ O / ",
        "     |   ",
        "   / O \\ ",
    };

    int frame_count = sizeof(frames) / sizeof(frames[0]);
    int frame = 0;
    int running = 1;

    SDL_Event event;
    Uint32 last = SDL_GetTicks();

    while (running) {
        // Handle window events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        Uint32 now = SDL_GetTicks();
        if (now - last > 100) { // ~10 FPS
            last = now;

            // Clear terminal (ANSI escape)
            printf("\033[2J\033[H");
            printf("ASCII Spinner:\n\n");
            printf("%s\n", frames[frame]);

            frame = (frame + 1) % frame_count;
            fflush(stdout);
        }

        SDL_Delay(1); // don’t burn CPU
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
