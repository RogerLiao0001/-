CFLAGS := `sdl2-config --libs --cflags` -ggdb3 -O0 --std=c99  -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

all: mysdl.c tomlread.c
	-gcc mysdl.c SDL_function.c toml.c -o mysdl $(CFLAGS)
clean:
	rm example
