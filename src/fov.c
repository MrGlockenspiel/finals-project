#include <math.h>

#include <rogue.h>

int getDistance(Position origin, Position target) {
    double dy, dx;
    int distance;

    dx = target.x - origin.x;
    dy = target.y - origin.y;

    distance = floor(sqrt((dx * dx) + (dy * dy)));

    return distance;
}

double getFloatDistance(Position origin, Position target) {
    char buffer[64];
    double dy, dx, distance;

    dx = target.x - origin.x;
    dy = target.y - origin.y;

    distance = sqrt((dx * dx) + (dy * dy));

    sprintf(buffer, "getFloatDistance: %f", distance);
    write_log(buffer);

    return distance;
}

int isInMap(int y, int x) {
    if ((0 < y && y < MAP_HEIGHT - 1) && (0 < x && x < MAP_WIDTH - 1)) {
        return 1;
    }
    return 0;
}

int getSign(int a) {
    return (a < 0) ? -1 : 1;
}

int lineOfSight(Position origin, Position target) {
    int t, x, y, abs_delta_x, abs_delta_y, sign_x, sign_y, delta_x, delta_y;

    delta_x = origin.x - target.x;
    delta_y = origin.y - target.y;

    abs_delta_x = abs(delta_x);
    abs_delta_y = abs(delta_y);

    sign_x = getSign(delta_x);
    sign_y = getSign(delta_y);

    x = target.x;
    y = target.y;

    if (abs_delta_x > abs_delta_y) {
        t = abs_delta_y * 2 - abs_delta_x;

        do {
            if (t >= 0) {
                y += sign_y;
                t -= abs_delta_x * 2;
            }

            x += sign_x;
            t += abs_delta_y * 2;

            if (x == origin.x && y == origin.y) {
                return true;
            }
        }
        while (map[y][x].transparent);

        return false;
    } else {
        t = abs_delta_x * 2 - abs_delta_y;

        do {
            if (t >= 0) {
                x += sign_x;
                t -= abs_delta_y * 2;
            }

            y += sign_y;
            t += abs_delta_x * 2;

            if (x == origin.x && y == origin.y) {
                return true;
            }
        }
        while (map[y][x].transparent);

        return false;
    }
}

void makeFOV(Entity* player, int radius) {
    int y, x, distance;
    Position target;

    
    map[player->pos.y][player->pos.x].visibility = 1;
        
    map[player->pos.y][player->pos.x].seen = 1;

    for (y = player->pos.y - radius; y < player->pos.y + radius; y++) {
        for (x = player->pos.x - radius; x < player->pos.x + radius; x++) {
            target.y = y;
            target.x = x;

            distance = getDistance(player->pos, target);

            if (distance < radius) {
                if (isInMap(y, x) && lineOfSight(player->pos, target)) {
                    map[y][x].visibility = 1;
                    map[y][x].seen = 1;
                }
            }
        }
    }
}

void clearFOV(Entity* player, int radius) {
    int y, x;

    for (y = player->pos.y - radius; y < player->pos.y + radius; y++) {
        for (x = player->pos.x - radius; x < player->pos.x + radius; x++) {
            if (isInMap(y, x)) {
                map[y][x].visibility = 0;
            }
        }
    }
}
