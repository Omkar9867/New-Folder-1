SDL_INCLUDE = C:/SDL-main/SDL-main/i686-w64-mingw32/include
SDL_LIB = C:/SDL-main/SDL-main/i686-w64-mingw32/lib
CFLAGS = -Wall -Wextra -pedantic -I$(SDL_INCLUDE)
LIBS = -L$(SDL_LIB) -lSDL3 -lm

something: main.cpp
	gcc $(CFLAGS) -o something main.cpp $(LIBS)