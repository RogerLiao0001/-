CFLAGS := `sdl2-config --libs --cflags` -ggdb3 -O0 --std=c99  -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

all: mysdl.c
	-gcc mysdl.c SDL_function.c toml.c -o mysdl $(CFLAGS)
	rm -f ai_script.toml
	gcc main.c cJSON.c -o main -lcurl -lm
	gcc test.c cJSON.c -o test -lcurl -lm
clean:
	rm example
