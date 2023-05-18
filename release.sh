gcc src/*.c -o roguelike -D_REENTRANT -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600 -O3 -mtune=native -march=native -I./inc/ -I/usr/include/ncursesw -lncursesw -lm -lSDL2
