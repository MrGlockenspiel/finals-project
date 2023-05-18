#include <stdlib.h>
#include <ncurses.h>

#include <rogue.h>

Entity *createProjectile() {
    Entity *newProjectile = calloc(1, sizeof(Entity));

    newProjectile->pos.y = MAP_HEIGHT - 1;
    newProjectile->pos.x = MAP_WIDTH - 1;
    newProjectile->ch = L"?";
    newProjectile->color = COLOR_PAIR(1);
    newProjectile->direction = NONE;

    return newProjectile;
}

void resetProjectile(Entity *proj) {
    clearFOV(proj, 2);
    proj->pos.y = 20;
    proj->pos.x = MAP_WIDTH + 13;
    proj->ch = L"|";
    proj->color = COLOR_PAIR(1);
    proj->direction = NONE;

    canFire = 1;
}

void fireProjectile(Entity *proj, Entity *owner) {
    Position newPos = { owner->pos.y, owner->pos.x };

    // anything related to sound effects is fundamentally fucked so
    // by commenting this out at least you get to hear the gib sounds
    //play_sound_effect("sound/effects/an944.wav");

    switch (owner->direction) {
        case UP:
            newPos.y--;
            proj->ch = L"|";
            proj->direction = UP;
            break;
        case DOWN:
            newPos.y++;
            proj->ch = L"|";
            proj->direction = DOWN;
            break;
        case LEFT:
            newPos.x--;
            proj->ch = L"-";
            proj->direction = LEFT;
            break;
        case RIGHT:
            newPos.x++;
            proj->ch = L"-";
            proj->direction = RIGHT;
            break;
        default:
            break;
    }

    if (map[newPos.y][newPos.x].walkable) {
        proj->pos.y = newPos.y;
        proj->pos.x = newPos.x;
        canFire = 0;
    }
}

void moveProjectile(Entity *proj) {
    Position newPos = { proj->pos.y, proj->pos.x };

    switch (proj->direction) {
        case UP:
            newPos.y--;
            proj->ch = L"|";
            proj->direction = UP;
            break;
        case DOWN:
            newPos.y++;
            proj->ch = L"|";
            proj->direction = DOWN;
            break;
        case LEFT:
            newPos.x--;
            proj->ch = L"-";
            proj->direction = LEFT;
            break;
        case RIGHT:
            newPos.x++;
            proj->ch = L"-";
            proj->direction = RIGHT;
            break;
        default:
            break;
    }

    if (newPos.y < MAP_HEIGHT && newPos.x < MAP_WIDTH) {
        if (map[newPos.y][newPos.x].walkable) {
            clearFOV(proj, 2);
            proj->pos.y = newPos.y;
            proj->pos.x = newPos.x;
            makeFOV(proj, 2);
        } else {
            resetProjectile(proj);
        }
    }
}
