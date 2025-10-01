#include <cstdio>
#include <cstdlib>
#include <SDL3/SDL.h>

template <typename T>
T max(T a, T b)
{
    return a < b ? b : a;
}

// Just for checking the error
int sdl(int code)
{
    if (code < 0)
    {
        fprintf(stderr, "SDL pooped itself: %s\n", SDL_GetError());
        abort();
    }
    return code;
}

template <typename T>
T *sdl(T *ptr)
{
    if (ptr == nullptr)
    {
        fprintf(stderr, "SDL pooped itself: %s\n", SDL_GetError());
        abort();
    }
    return ptr;
}

float TILE_SIZE = 69;

enum class Tile
{
    Empty = 0,
    Wall
};

constexpr int LEVEL_WIDTH = 5;
constexpr int LEVEL_HEIGHT = 5;

Tile level[LEVEL_HEIGHT][LEVEL_WIDTH] = {
    {Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty},
    {Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty},
    {Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty},
    {Tile::Wall, Tile::Wall, Tile::Wall, Tile::Empty, Tile::Empty},
    {Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty},
};

void renderLevel(SDL_Renderer *renderer){
    for(int y = 0; y < LEVEL_HEIGHT; ++y){
        for(int x = 0; x < LEVEL_WIDTH; ++x){
            switch (level[y][x])
            {
            case Tile::Empty:{
            }break;
            case Tile::Wall:{
                sdl(SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255));
                SDL_FRect rect = {
                    x * TILE_SIZE,
                    y * TILE_SIZE,
                    TILE_SIZE,
                    TILE_SIZE
                }; 
                sdl(SDL_RenderFillRect(renderer, &rect));
            }break;
            default:
                break;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    (void)argc; // Suppress unused parameter warning
    (void)argv; // Suppress unused parameter warning

    sdl(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS));
    SDL_Window *window = sdl(SDL_CreateWindow("something", 800, 600, SDL_WINDOW_RESIZABLE));
    // Not just above will work, we have to render by renderer
    SDL_Renderer *renderer = sdl(SDL_CreateRenderer(window, SDL_GetRenderDriver(0)));

    bool quit = false;
    while (!quit) //! This is basically an event loop we are creating
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_EVENT_QUIT:
            {
                quit = true;
            }
            break;
            }
        }
        // Update state
        // Render state
        sdl(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255));
        sdl(SDL_RenderClear(renderer));
        
        renderLevel(renderer);
        SDL_RenderPresent(renderer);
    }
    SDL_Quit();

    return 0;
}