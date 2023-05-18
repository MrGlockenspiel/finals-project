gcc src/*.c -fsanitize=address -g -O0 -o roguelike -I./inc/ $(pkg-config --libs --cflags ncursesw sdl2) -lm
