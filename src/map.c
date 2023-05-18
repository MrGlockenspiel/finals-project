#include <stdlib.h>
#include <ncurses.h>

#include <rogue.h>
#include <levels.h>

#define RAND(min, max) ((rand() % (max)) + min)

Tile** createMapTiles(void) {
    Tile **tiles = calloc(MAP_HEIGHT, sizeof(Tile*));

    for (int y = 0; y < MAP_HEIGHT; y++) {
        tiles[y] = calloc(MAP_WIDTH + 1, sizeof(Tile));
        for (int x = 0; x < MAP_WIDTH; x++) {
            tiles[y][x].ch = L"#";
            tiles[y][x].color = COLOR_PAIR(VISIBLE_COLOR);
            tiles[y][x].walkable = 0;
            tiles[y][x].transparent = 0;
            tiles[y][x].visibility = 0;
            tiles[y][x].seen = 0;
            tiles[y][x].gore = 0;
        }
    }
    return tiles;
}

Position setupMap(void) {
    //Position start_pos = { 6, 6 };
    
    /*for (int y = 5; y < 15; y++) {
        for (int x = 40; x < 60; x++) {
            map[y][x].ch = L".";
            map[y][x].walkable = true;
        }
    }*/

    /*for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (level1[x][y] == '#') {
                map[x][y].ch = L"\u2588";
                map[x][y].walkable = 0;
            } else if (level1[x][y] == '$') {
                map[x][y].ch = L"$";
                map[x][y].walkable = 1;
            } else if (level1[x][y] == 'W') {
                map[x][y].ch = L"~";
                map[x][y].walkable = 2;
            } else if (level1[x][y] != 0) {
                map[x][y].ch = L" ";
                map[x][y].walkable = 1;
            }
        }
    }*/

    int y, x, height, width, n_rooms;
    n_rooms = (rand() % 11) + 5;
    Room *rooms = calloc(n_rooms, sizeof(Room));
    Position start_pos;

    for (int i = 0; i < n_rooms; i++) {
        y = RAND(1, MAP_HEIGHT - 10);
        x = RAND(1, MAP_WIDTH - 20);
        height = RAND(3, 7);
        width = RAND(3, 15);
        rooms[i] = createRoom(y, x, height, width);
        addRoomToMap(rooms[i]);

        if (i > 0) {
            connectRoomCenters(rooms[i - 1].center, rooms[i].center);
        }
    }

    start_pos.y = rooms[0].center.y;
    start_pos.x = rooms[0].center.x;

    //enemy = create_enemy(rooms[1].center);

    free(rooms);
    
    return start_pos;
}

void freeMap(void) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        free(map[y]);
    }
    free(map);
}
