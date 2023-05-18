gcc src/*.c -g --std=gnu99 -o roguelike -I./inc/ $(pkg-config --libs --cflags ncursesw sdl2) -lm
