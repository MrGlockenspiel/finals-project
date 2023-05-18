#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>

#include <rogue.h>

Entity *createPlayer(Position start_pos) {
    Entity *newPlayer = calloc(1, sizeof(Entity));

    newPlayer->pos.y = start_pos.y;
    newPlayer->pos.x = start_pos.x;
    //newPlayer->ch = L"\u0D9E";
    newPlayer->ch = L"@";
    //newPlayer->color = COLOR_PAIR(VISIBLE_COLOR);
    newPlayer->color = COLOR_PAIR(1);
    newPlayer->direction = UP;
    newPlayer->alive = 1;

    return newPlayer;
}

void handleInput(int input) {
    if (!player->alive) return;

    Position newPos = { player->pos.y, player->pos.x };

    switch (input) {
        // move up
        case 'w':
            newPos.y--;
            player->direction = UP;
            break;
        // move down
        case 's':
            newPos.y++;
            player->direction = DOWN;
            break;
        // move left
        case 'a':
            newPos.x--;
            player->direction = LEFT;
            break;
        // move right
        case 'd':
            newPos.x++;
            player->direction = RIGHT;
            break;
        case ' ':
            if (canFire) fireProjectile(projectile, player);
            break;
        case 'e':
            interact();
            break;
        default:
            break;
    }

    movePlayer(newPos);
}

void movePlayer(Position newPos) {
    if (map[newPos.y][newPos.x].walkable) {
        clearFOV(player, 15);
        player->pos.y = newPos.y;
        player->pos.x = newPos.x;
        makeFOV(player, 15);
    }
}

void interact() {
    spawn_dialog("NOTHING HERE.");
}

void checkHealth() {
    if (health < 1) die();
}

void die() {
    player->alive = 0;

    // this doesnt work, ignoring the problem
    attron(COLOR_PAIR(GAME_OVER_COLOR));
    for(int y = 0; y < SCREEN_HEIGHT; y++) {
        for(int x = 0; x < SCREEN_WIDTH; x++) {
            mvaddch(y, x, ' ');
        }
    }
    attroff(COLOR_PAIR(GAME_OVER_COLOR));

    //usleep(1500000);
    
    attron(COLOR_PAIR(GAME_OVER_COLOR3));
    for(int y = 0; y < SCREEN_HEIGHT; y++) {
        for(int x = 0; x < SCREEN_WIDTH; x++) {
            mvaddch(y, x, ' ');
        }
    }
    attroff(COLOR_PAIR(GAME_OVER_COLOR3));
    
    
    attron(COLOR_PAIR(GAME_OVER_COLOR));
    mvprintw(SCREEN_HEIGHT / 2, (SCREEN_WIDTH - strlen("GAME OVER.")) / 2, "GAME OVER.");
    attroff(COLOR_PAIR(GAME_OVER_COLOR));

    refresh();
    usleep(5000000);
    getch();
    closeGame();
}
