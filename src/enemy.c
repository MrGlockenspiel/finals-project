#include <ncurses.h>

#include <rogue.h>

Direction pick_direction() {
    return rand_lut() % 20;
}

void move_enemy(Enemy *enemy) {
    char buffer[64];
    Position newPos = { enemy->pos.y, enemy->pos.x };

    sprintf(buffer, "AI: enemy->pos: (%d, %d)", enemy->pos.x, enemy->pos.y);
    write_log(buffer);

    switch(pick_direction()) {
        case UP:
            newPos.y--;
            enemy->direction = UP;
            break;
        case DOWN:
            newPos.y++;
            enemy->direction = DOWN;
            break;
        case LEFT:
            newPos.x--;
            enemy->direction = LEFT;
            break;
        case RIGHT:
            newPos.x++;
            enemy->direction = RIGHT;
            break;
        case NONE:
        default:
            break;
    }

    if (newPos.y < MAP_HEIGHT - 1 && newPos.x < MAP_WIDTH - 1 && newPos.x > 1 && newPos.y > 1) {
        if (map[newPos.y][newPos.x].walkable) {
            enemy->pos.y = newPos.y;
            enemy->pos.x = newPos.x;
        }
    }

    if ((enemy->pos.y == projectile->pos.y && enemy->pos.x == projectile->pos.x)
        || (newPos.y == projectile->pos.y && newPos.x == projectile->pos.y)) {
        kill_enemy(enemy);
        resetProjectile(projectile);
    }

    if ((enemy->pos.y == player->pos.y && enemy->pos.x == player->pos.x)
        || (newPos.y == player->pos.y && newPos.x == player->pos.y)) {
        health--;
        checkHealth();
    }
}

void move_enemies(Enemy **enemies, int num) {
    char buffer[16];
    for (int i = 0; i < num; i++) {
        sprintf(buffer, "Moving enemy %d", i);
        write_log(buffer);
        if(enemies[i]->state != DEAD) move_enemy(enemies[i]);
    }
}

Enemy *create_enemy() {
    Enemy *newEnemy = calloc(1, sizeof(Enemy));
    Position newPos;
    unsigned char picked;
    char buffer[64];

    while(!picked) {
        newPos.x = rand_lut() % MAP_WIDTH - 1;
        newPos.y = rand_lut() & MAP_HEIGHT - 1;

        if (map[newPos.y][newPos.x].walkable) {
            picked = 1;
        }
    }

    newEnemy->pos.y = newPos.y;
    newEnemy->pos.x = newPos.x;
    newEnemy->ch = L"X";
    newEnemy->color = COLOR_PAIR(VISIBLE_COLOR);
    newEnemy->direction = NONE;
    newEnemy->state = WAIT;
    newEnemy->target = NULL;

    return newEnemy;
}

Enemy **create_enemy_array(int num) {
    Enemy **enemies = calloc(num, sizeof(Enemy*));
    
    for (int i = 0; i < num; i++) {
        enemies[i] = create_enemy();
    }

    return enemies;
}

void kill_enemy(Enemy *enemy) {
    char path[64];

    Tile skull;
    skull.ch = L"o";
    skull.walkable = 1;
    skull.color = COLOR_PAIR(GORE_COLOR);
    skull.transparent = 1;
    skull.visibility = GORE;
    skull.seen = 1;
    skull.gore = 1;

    Tile giblet;
    giblet.ch = L".";
    giblet.walkable = 1;
    giblet.color = COLOR_PAIR(GORE_COLOR);
    giblet.transparent = 1;
    giblet.visibility = GORE;
    giblet.seen = 1;
    giblet.gore = 1;

    Tile splatter;
    splatter.ch = L" ";
    splatter.walkable = 1;
    splatter.color = COLOR_PAIR(GORE_COLOR);
    splatter.transparent = 1;
    splatter.visibility = GORE;
    splatter.seen = 1;
    splatter.gore = 1;

    Tile gore[4] = { splatter, skull, splatter, giblet };

    Tile* t;

    for (int i = 0; i < rand_lut() % 5; i++) {
        Direction splatter_dir = rand_lut() % 5;

        int x = enemy->pos.x;
        int y = enemy->pos.y;

        switch (splatter_dir) {
            case UP:
                if (y + 1 < MAP_HEIGHT) t = &map[y + 1][x];
                break;
            case DOWN:
                if (y - 1 > 0) t = &map[y - 1][x];
                break;
            case LEFT:
                if (x - 1 > 0) t = &map[y][x - 1];
                break;
            case RIGHT:
                if (x + 1 < MAP_WIDTH) t = &map[y][x + 1];
                break;
            default:
                t = &map[y][x];
                break;
        }
    }

    if (map[enemy->pos.y][enemy->pos.x].ch == L"#") {
        t->color = COLOR_PAIR(GORE_COLOR);
    } else {
        *t = gore[rand_lut() % 3];
    }

    enemy->ch = L"%";
    enemy->color = COLOR_PAIR(GORE_COLOR);
    enemy->state = DEAD;

    sprintf(path, "sound/effects/flesh_%d.wav", rand_lut() % 4);
    play_sound_effect(path);
}
