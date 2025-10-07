#include <cstdio>
#include <cstdlib>
#include <SDL3/SDL.h>
#include <png.h>

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

const int LEVEL_WIDTH = 5;
const int LEVEL_HEIGHT = 5;

Tile level[LEVEL_HEIGHT][LEVEL_WIDTH] = {
    {Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty},
    {Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty},
    {Tile::Empty, Tile::Wall, Tile::Empty, Tile::Empty, Tile::Empty},
    {Tile::Wall, Tile::Wall, Tile::Wall, Tile::Empty, Tile::Empty},
    {Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty, Tile::Empty},
};

struct Tile_Texture{
    SDL_FRect srcrect;
    SDL_Texture *texture;
};

void render_tile_texture(SDL_Renderer *renderer, Tile_Texture texture, int x, int y){
    SDL_FRect dstrect = {x, y, TILE_SIZE, TILE_SIZE}; 
    sdl(SDL_RenderTexture(renderer, texture.texture, &texture.srcrect, &dstrect ));
}

void renderLevel(SDL_Renderer *renderer, Tile_Texture wall_texture){
    for(int y = 0; y < LEVEL_HEIGHT; ++y){
        for(int x = 0; x < LEVEL_WIDTH; ++x){
            switch (level[y][x])
            {
            case Tile::Empty:{
            }break;
            case Tile::Wall:{
                sdl(SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255));
                // SDL_FRect rect = {
                //     x * TILE_SIZE,
                //     y * TILE_SIZE,
                //     TILE_SIZE,
                //     TILE_SIZE
                // }; 
                render_tile_texture(renderer, wall_texture, x * TILE_SIZE, y * TILE_SIZE);
                // sdl(SDL_RenderFillRect(renderer, &rect));
            }break;
            default:
                break;
            }
        }
    }
}

SDL_Texture *load_texture_from_png_file(const char *image_filename, SDL_Renderer *renderer){
    png_image image;
    memset(&image, 0, sizeof(image));

    //2. set the version field to PNG_IMAGE_VERSION
    image.version = PNG_IMAGE_VERSION;

    //3. Appropriate begin to read image from file 
    if(!png_image_begin_read_from_file(&image, image_filename)){
        fprintf(stderr, "Could not read file %s : %s\n", image_filename, image.message);
        abort();
    }
    printf("Width: %d, Height: %d\n", image.width, image.height);

    //4. Set the 'png_image' format member to required sample format
    image.format = PNG_FORMAT_RGBA;

    //5. Allocating buffer for the image
    uint32_t *image_pixels = (uint32_t *) malloc(sizeof(uint32_t) * image.width * image.height);

    //6. Calling img finish read to read the image
    if(!png_image_finish_read(&image, nullptr, image_pixels, 0, nullptr)){
        fprintf(stderr, "libpng pooped itself %s\n", image.message);
        abort();
    }

    //7. Make the sdl_surface created by the rgb_surface
    int pitch = image.width * 4; // 4 bytes per pixel for RGBA
    SDL_Surface *image_surface = sdl(SDL_CreateSurfaceFrom(image.width, image.height, SDL_PIXELFORMAT_RGBA32, image_pixels, pitch));

    //8. Make the texture of the surface which is be render for the pixel
    SDL_Texture *image_texture = sdl(SDL_CreateTextureFromSurface(renderer, image_surface));

    //9, Destorying the texture
    SDL_DestroySurface(image_surface);

    return image_texture;
}

int main(int argc, char *argv[])
{
    (void)argc; // Suppress unused parameter warning
    (void)argv; // Suppress unused parameter warning

    sdl(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS));
    SDL_Window *window = sdl(SDL_CreateWindow("something", 800, 600, SDL_WINDOW_RESIZABLE));
    // Not just above will work, we have to render by renderer
    SDL_Renderer *renderer = sdl(SDL_CreateRenderer(window, SDL_GetRenderDriver(0)));
    
    //1. Declare 'png_image' struc  ture on the stack
    const char* tileset_filename = "fantasy_tiles.png"; //Todo: Replace with own assets

    SDL_Texture *tileset_texture = load_texture_from_png_file(tileset_filename, renderer);

    // 10. Making the actual wall texture form the Tile texture struct
    Tile_Texture wall_texture = {
        .srcrect = {120, 128, 16, 16},
        .texture = tileset_texture
    };
    
    //--------------- Walking man effect-----------------------
    const char* character_filename = "walking-12px.png"; //Todo: Replace with own assets
    SDL_Texture *player_walking_texture = load_texture_from_png_file(character_filename, renderer);

    constexpr int walking_frame_size = 48;
    constexpr int walking_animation_count = 4;
    Tile_Texture walking_animation[walking_animation_count];

    for(int i = 0; i < walking_animation_count; ++i){
        walking_animation[i].srcrect = {
            .x = static_cast<float>(i * walking_frame_size),
            .y = 0.0f,
            .w = static_cast<float>(walking_frame_size),
            .h = static_cast<float>(walking_frame_size)
        };
        walking_animation[i].texture = player_walking_texture;
    }

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
        
        // sdl(SDL_RenderTexture(renderer, tileset_texture, nullptr, nullptr ));
        renderLevel(renderer, wall_texture);
        render_tile_texture(renderer, walking_animation[0], 0, 0);
        SDL_RenderPresent(renderer);
    }
    SDL_Quit();

    return 0;
}