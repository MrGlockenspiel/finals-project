#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>
#include <sys/time.h>

#include <rogue.h>

#define FRAME_CAP 20

//#define ACTIVATE_LINUX_INTEGRATION
//#define VSYNC
//#define NODELAY

double deltaTime, frameTime;
char fps[16];

int cursesSetup(void) {
    WINDOW *win;
    setlocale(LC_ALL, "");
    initscr();
    
    #ifdef NODELAY
    nodelay(stdscr, TRUE);
    #endif
    
    noecho();
    curs_set(0);

    //getmaxyx(stdscr, SCREEN_HEIGHT, SCREEN_WIDTH);

    //ui = newwin(SCREEN_HEIGHT, SCREEN_WIDTH - MAP_WIDTH - 1, 0, MAP_WIDTH + 1);

    if (has_colors()) {
        start_color();

        init_pair(VISIBLE_COLOR, COLOR_WHITE, COLOR_BLACK);
        init_pair(SEEN_COLOR, COLOR_BLUE, COLOR_BLACK);
        //init_pair(GORE_COLOR, COLOR_RED, COLOR_BLACK);
        init_pair(GORE_COLOR, COLOR_WHITE, COLOR_RED);
        init_pair(GAME_OVER_COLOR, COLOR_RED, COLOR_BLACK);
        init_pair(GAME_OVER_COLOR2, COLOR_RED, COLOR_RED);
        init_pair(GAME_OVER_COLOR3, COLOR_BLACK, COLOR_BLACK);

        return 1;
    } else {
        mvaddwstr(20, 50, L"Your terminal does not support color. Cannot start game. ");
        getch();
        return 0;
    } 
}

void gameLoop(void) {
    int ch;
    int frames = 0;
    struct timeval lastTime, currTime;
    double deltaTime, fps;

    #ifdef ACTIVATE_LINUX_INTEGRATION
    char temp[128];
    #endif

    gettimeofday(&lastTime, NULL);

    makeFOV(player, 15);

    drawAll();

    while (ch = getch()) {
        if (ch == 'q') {
            break;
        }

        // handle input and draw the frame
        handleInput(ch);
        drawAll();
        
        // calculate FPS and frame time
        gettimeofday(&currTime, NULL);
        deltaTime = (currTime.tv_sec - lastTime.tv_sec) + (currTime.tv_usec - lastTime.tv_usec) / 1000000.0;
        fps = 1.0 / deltaTime;
        lastTime = currTime;
        double frameTime = deltaTime * 1000.0;  // convert to milliseconds
        
        // print FPS and frame time to the screen
        mvprintw(MAP_HEIGHT + 1, 0, "FPS: %.2f", fps);

        #ifdef ACTIVATE_LINUX_INTEGRATION
        sprintf(temp, "activate-linux -t \"FPS: %.2f\" -m \"Frametime: %.2fms\" &", fps, frameTime);
        system("pkill -9 activate-linux");
        system(temp);
        #endif
        
        mvprintw(MAP_HEIGHT + 2, 0, "Frame time: %.2fms", frameTime);
        
        // wait for the next frame :clueless: (this is a joke)
        #ifdef VSYNC
        if (fps > FRAME_CAP) usleep(((fps - FRAME_CAP) / 60) * 1000);
        #endif
    }
}

void closeGame(void) {
    endwin();
    free(player);
    free(projectile);

    for (int i = 0; i < 4; i++) {
        free(enemies[i]);
    }

    free(enemies);
    
    #ifdef ACTIVATE_LINUX_INTEGRATION
    system("pkill -9 activate-linux &");
    #endif
}
