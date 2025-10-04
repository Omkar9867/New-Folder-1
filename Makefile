SDL_INCLUDE = C:/SDL-main/SDL-main/i686-w64-mingw32/include
SDL_LIB = C:/SDL-main/SDL-main/i686-w64-mingw32/lib
PNG_INCLUDE = C:/msys64/mingw32/include
PNG_LIB = C:/msys64/mingw32/lib

CFLAGS = -Wall -Wextra -pedantic -g -gdwarf-4 -I$(SDL_INCLUDE) -I$(PNG_INCLUDE)
LIBS = -L$(SDL_LIB) -L$(PNG_LIB) -lSDL3 -lpng16 -lm

something: main.cpp
	g++ $(CFLAGS) -o something main.cpp $(LIBS)