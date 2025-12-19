#include <SDL2/SDL.h>
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define PADDLE_WIDTH   20
#define PADDLE_HEIGHT  120
#define PADDLE_SPEED   6

#define BALL_SIZE      15
#define BALL_SPEED     3

void move_paddle(SDL_Rect *p, int dy) {
    p->y += dy;

    if (p->y < 0)
        p->y = 0;
    if (p->y + p->h > WINDOW_HEIGHT)
        p->y = WINDOW_HEIGHT - p->h;
}

void move_ball(SDL_Rect *b, int *dx, int *dy) {
    b->x += *dx;
    b->y += *dy;

    
    if (b->y <= 0) {
        b->y = 0;
        *dy = -*dy;
    }
    if (b->y + b->h >= WINDOW_HEIGHT) {
        b->y = WINDOW_HEIGHT - b->h;
        *dy = -*dy;
    }

    
    if (b->x < 0 || b->x > WINDOW_WIDTH) {
        b->x = (WINDOW_WIDTH - BALL_SIZE) / 2;
        b->y = (WINDOW_HEIGHT - BALL_SIZE) / 2;

        *dx = (*dx > 0) ? -BALL_SPEED : BALL_SPEED;
        *dy = BALL_SPEED;
    }
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "Pong",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    SDL_Rect p1 = { 40, (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2,
                    PADDLE_WIDTH, PADDLE_HEIGHT };

    SDL_Rect p2 = { WINDOW_WIDTH - 40 - PADDLE_WIDTH,
                    (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2,
                    PADDLE_WIDTH, PADDLE_HEIGHT };

    SDL_Rect ball = {
        (WINDOW_WIDTH - BALL_SIZE) / 2,
        (WINDOW_HEIGHT - BALL_SIZE) / 2,
        BALL_SIZE,
        BALL_SIZE
    };

    int ball_dx = BALL_SPEED;
    int ball_dy = BALL_SPEED;

    int running = 1;
    SDL_Event event;

    while (running) {

       
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;
            if (event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_ESCAPE)
                running = 0;
        }

        
        const Uint8 *keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_W])
            move_paddle(&p1, -PADDLE_SPEED);
        if (keys[SDL_SCANCODE_S])
            move_paddle(&p1,  PADDLE_SPEED);

        if (keys[SDL_SCANCODE_UP])
            move_paddle(&p2, -PADDLE_SPEED);
        if (keys[SDL_SCANCODE_DOWN])
            move_paddle(&p2,  PADDLE_SPEED);

        
        move_ball(&ball, &ball_dx, &ball_dy);

        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &p1);
        SDL_RenderFillRect(renderer, &p2);
        SDL_RenderFillRect(renderer, &ball);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
