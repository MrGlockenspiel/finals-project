#include <stdlib.h>
#include <math.h>

#include <rogue.h>

#define HEART_FULL L"\u2764"

void drawMap(void) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (map[y][x].gore) {
                attron(map[y][x].color);
                mvaddwstr(y, x, map[y][x].ch);
                attroff(map[y][x].color);
            } else if (map[y][x].visibility) {
                attron(map[y][x].color);
                mvaddwstr(y, x, map[y][x].ch);
                attroff(map[y][x].color);
            } else if (map[y][x].seen) {
                attron(COLOR_PAIR(SEEN_COLOR));
                mvaddwstr(y, x, map[y][x].ch);
                attroff(COLOR_PAIR(SEEN_COLOR));
            } else {
                mvaddwstr(y, x, L" ");
            }
        }
    }
}

void drawEntity(Entity *entity) {
    attron(entity->color);
    mvaddwstr(entity->pos.y, entity->pos.x, entity->ch);
    attroff(entity->color);
}

void drawEnemy(Enemy *enemy) {
    attron(enemy->color);
    mvaddwstr(enemy->pos.y, enemy->pos.x, enemy->ch);
    attroff(enemy->color);
}

void drawPlayer(Entity* entity) {
    attron(map[entity->pos.y][entity->pos.x].color);
    mvaddwstr(entity->pos.y, entity->pos.x, entity->ch);
    attroff(map[entity->pos.y][entity->pos.x].color);
}

void drawEnemies(Enemy **enemies, int num) {
    for (int i = 0; i < num; i++) {
        if (map[enemies[i]->pos.y][enemies[i]->pos.x].visibility || 
            enemies[i]->state == DEAD) {
            drawEnemy(enemies[i]);
        }
    }
}

void drawAll(void) {
    clear();
    drawMap();
    //firstPersonDrawMap();

    move_enemies(enemies, 4);

    drawEnemies(enemies, 4);

    drawPlayer(player);

    drawUI();

    moveProjectile(projectile);
    drawEntity(projectile);
    
}

void drawUI() {
    mvprintw(20, MAP_WIDTH + 3, "AMMO:   [   ]");
    mvprintw(21, MAP_WIDTH + 3, "HEALTH: [          ]");

    attron(COLOR_PAIR(GAME_OVER_COLOR));
    for (int i = 0; i < health; i++) {
        mvaddwstr(21, MAP_WIDTH + 13 + (i * 3), L"<3");
    }
    attroff(COLOR_PAIR(GAME_OVER_COLOR));
    
    box(stdscr, 0, 0);
    mvvline(1, MAP_WIDTH, 0, MAP_HEIGHT - 2);
}

/*
void firstPersonDrawMap() {
    int fov = 60;  // field of view in degrees
    int distance = 10;  // max rendering distance

    // clear the screen
    werase(stdscr);

    // loop through each row of the screen
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        // calculate the vertical angle of the ray
        float rayAngle = (float)fov / 2 - ((float)fov / SCREEN_HEIGHT) * y;
        
        // loop through each column of the screen
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            // calculate the horizontal angle of the ray
            float angle = player->view_x + ((float)fov / SCREEN_WIDTH) * x - (float)fov / 2;

            // calculate the distance to the wall
            float distanceToWall = 0;
            int hitWall = 0;
            int wallX, wallY;

            while (!hitWall && distanceToWall < distance) {
                distanceToWall += 0.1;

                wallX = player->pos.x + distanceToWall * cos(angle);
                wallY = player->pos.y + distanceToWall * sin(angle);

                if (wallX < 0 || wallX >= MAP_WIDTH || wallY < 0 || wallY >= MAP_HEIGHT) {
                    hitWall = 1;
                    distanceToWall = distance;
                } else if (map[wallY][wallX].transparent == 0) {
                    hitWall = 1;
                }
            }

            // calculate the height of the wall based on the distance
            int wallHeight = SCREEN_HEIGHT / distanceToWall;

            // calculate the color of the wall
            int color = map[wallY][wallX].color;

            // draw the column of the wall
            for (int i = 0; i < SCREEN_HEIGHT; i++) {
                if (i < SCREEN_HEIGHT / 2 - wallHeight / 2 || i > SCREEN_HEIGHT / 2 + wallHeight / 2) {
                    mvaddch(i, x, ' ');
                } else {
                    attron(COLOR_PAIR(color));
                    mvaddch(i, x, '|');
                    attroff(COLOR_PAIR(color));
                }
            }
        }
    }

    // draw the player at the center of the screen
    drawEntity(player);

    // refresh the screen
    refresh();
}
*/
