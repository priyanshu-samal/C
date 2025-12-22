#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#define WIDTH  1000
#define HEIGHT 800

#define COLOR_EARTH (SDL_Color){0, 0, 255, 255}
#define COLOR_SUN   (SDL_Color){255, 200, 50, 255}
#define COLOR_RAY   (SDL_Color){255, 180, 80, 255}

typedef struct Circle {
    double x;
    double y;
    double radius;
} Circle;


double dist(double x1, double y1, double x2, double y2) {
    double dx = x1 - x2;
    double dy = y1 - y2;
    return sqrt(dx * dx + dy * dy);
}

void FillCircle(SDL_Surface *surface, Circle c, SDL_Color color) {
    Uint32 col = SDL_MapRGB(surface->format, color.r, color.g, color.b);

    SDL_LockSurface(surface);
    Uint32 *pixels = (Uint32 *)surface->pixels;
    int pitch = surface->pitch / 4;

    for (int y = 0; y < surface->h; y++) {
        for (int x = 0; x < surface->w; x++) {
            if (dist(x, y, c.x, c.y) <= c.radius) {
                pixels[y * pitch + x] = col;
            }
        }
    }

    SDL_UnlockSurface(surface);
}

void DrawSunRays(SDL_Surface *surface, Circle sun, Circle earth) {

    Uint32 rayCol = SDL_MapRGB(
        surface->format,
        COLOR_RAY.r, COLOR_RAY.g, COLOR_RAY.b
    );

    SDL_LockSurface(surface);
    Uint32 *pixels = (Uint32 *)surface->pixels;
    int pitch = surface->pitch / 4;

    int ray_count = 720;
    double max_len = 900.0;

    for (int i = 0; i < ray_count; i++) {
        double angle = (2.0 * M_PI * i) / ray_count;
        double dx = cos(angle);
        double dy = sin(angle);

        for (double t = sun.radius; t < max_len; t += 1.0) {

            int x = (int)(sun.x + dx * t);
            int y = (int)(sun.y + dy * t);

            if (x < 0 || x >= surface->w ||
                y < 0 || y >= surface->h)
                break;

            
            if (dist(x, y, earth.x, earth.y) <= earth.radius)
                break;

            pixels[y * pitch + x] = rayCol;
        }
    }

    SDL_UnlockSurface(surface);
}

int main(int argc, char *argv[]) {

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "Sharp Rays",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        SDL_WINDOW_SHOWN
    );

    SDL_Surface *surface = SDL_GetWindowSurface(window);

    Circle sun   = {500, 400, 140};
    Circle earth = {750, 400, 80};

    int running = 1;
    int dragging_sun = 0;
    int dragging_earth = 0;

    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT)
                running = 0;

            if (event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_ESCAPE)
                running = 0;

            
            if (event.type == SDL_MOUSEBUTTONDOWN &&
                event.button.button == SDL_BUTTON_LEFT) {

                double d_sun   = dist(event.button.x, event.button.y, sun.x, sun.y);
                double d_earth = dist(event.button.x, event.button.y, earth.x, earth.y);

                if (d_sun <= sun.radius)
                    dragging_sun = 1;
                else if (d_earth <= earth.radius)
                    dragging_earth = 1;
            }

            
            if (event.type == SDL_MOUSEBUTTONUP &&
                event.button.button == SDL_BUTTON_LEFT) {
                dragging_sun = 0;
                dragging_earth = 0;
            }

            
            if (event.type == SDL_MOUSEMOTION &&
                event.motion.state & SDL_BUTTON_LMASK) {

                if (dragging_sun) {
                    sun.x = event.motion.x;
                    sun.y = event.motion.y;
                }
                else if (dragging_earth) {
                    earth.x = event.motion.x;
                    earth.y = event.motion.y;
                }
            }
        }

        SDL_FillRect(surface, NULL,
            SDL_MapRGB(surface->format, 0, 0, 0));

        DrawSunRays(surface, sun, earth);
        FillCircle(surface, sun, COLOR_SUN);
        FillCircle(surface, earth, COLOR_EARTH);

        SDL_UpdateWindowSurface(window);
        SDL_Delay(16);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
