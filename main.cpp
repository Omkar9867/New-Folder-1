#include <cstdio>
#include <cstdlib>
#include <SDL3/SDL.h>

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

constexpr int TILE_SIZE = 69;

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

        int windowWidth = 0;
        int windowHeight = 0;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        // to get column and row just divide the window size with tile
        int columns = windowWidth / TILE_SIZE;
        int rows = windowHeight / TILE_SIZE;
        sdl(SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255));
        for (int row = 0; row < rows; row++)
        {
            sdl(SDL_RenderLine(renderer, 0, (row+1) * TILE_SIZE, windowWidth, (row+1) * TILE_SIZE));
        }
        for (int column = 0; column < columns; column++)
        {
            sdl(SDL_RenderLine(renderer, (column+1) * TILE_SIZE, 0 , (column+1) * TILE_SIZE, windowHeight));
        }

        SDL_RenderPresent(renderer);
    }
    SDL_Quit();

    return 0;
}