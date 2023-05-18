#include <stdlib.h>
#include <ncurses.h>

#include <rogue.h>

void spawn_dialog(char* string) {
    int height = 10;
    int width = 40;
    int x = 2;
    int y = 1;
    int start_y = (LINES - height) / 2;
    int start_x = (COLS - width) / 2;

    WINDOW* win = newwin(height, width, start_y, start_x);
    box(win, 0, 0);
    wrefresh(win);

    char* p = string;
    while (*p) {
        mvwprintw(win, y, x, "%c", *p);
        wrefresh(win);
        play_char(p[0]);
        p++, x++;
    }

    getch();

    delwin(win);
}
