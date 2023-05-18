#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include <time.h>

#include <rogue.h>

int SCREEN_HEIGHT = 24;
int SCREEN_WIDTH  = 80;

int MAP_HEIGHT    = 24;
int MAP_WIDTH     = 60;

Entity*        player;
Entity*        projectile;
Enemy**        enemies;
Tile**         map;
unsigned char  canFire;
unsigned char  health;

WINDOW* ui;

int main(void) {
    Position start_pos;
    int compatibleTerm;
    canFire = 1;
    health = 3;

    clear_log();

    compatibleTerm = cursesSetup();

    if (compatibleTerm) {
        srand(time(NULL));
        write_log("Init: Program init");
        
        map = createMapTiles();
        start_pos = setupMap();
        player = createPlayer(start_pos);
        write_log("Init: Map setup complete");

        projectile = createProjectile();
        enemies = create_enemy_array(4);
        write_log("Init: Entity creation complete");

        init_audio();
        write_log("Init: Audio engine started");

        load_music("sound/music/officemachines.wav");
        play_music();
        
        gameLoop();

        closeGame();
    } else {
        endwin();
    }
    
    return 0;
}

double clockToMilliseconds(clock_t ticks) {
    // units/(units/time) => time (seconds) * 1000 = milliseconds
    return (ticks / (double)CLOCKS_PER_SEC) * 1000.0;
}
