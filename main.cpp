#include <cstdio>
#include <cstdlib>
#include <SDL3/SDL.h>

// Just for checking the error
int sdl(int code){
    if(code < 0){
        fprintf(stderr, "SDL pooped itself: %s\n", SDL_GetError());
        abort();
    }
    return code;
}

template <typename T>
T *sdl(T *ptr){
    if(ptr == nullptr){
        fprintf(stderr, "SDL pooped itself: %s\n", SDL_GetError());
        abort();
    }
    return ptr;
}

int main(int argc, char *argv[]){
    sdl(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS));
    SDL_Window *window = sdl(SDL_CreateWindow("something", 800, 600, SDL_WINDOW_RESIZABLE));
    SDL_Quit();

    return 0;
}