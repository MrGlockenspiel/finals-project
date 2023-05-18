#ifndef ROGUE_H
#define ROGUE_H

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

// color pairs
#define VISIBLE_COLOR 1
#define SEEN_COLOR 2
#define GORE_COLOR 3
#define GAME_OVER_COLOR 4
#define GAME_OVER_COLOR2 5
#define GAME_OVER_COLOR3 6

typedef enum {
    UP,
    RIGHT,
    DOWN,
    LEFT,
    NONE,
} Direction;

typedef enum {
    WAIT,
    CHASE,
    DEAD,
} AIstate;

typedef enum {
    WALL,
    FLOOR,
    GORE,
} Visibility;

typedef struct {
    int y;
    int x;
} Position;

typedef struct {
    wchar_t *ch;
    unsigned char walkable;
    int color;
    unsigned char transparent;
    Visibility visibility;
    unsigned char seen;
    unsigned char gore;
} Tile;

typedef struct {
    int height;
    int width;
    Position pos;
    Position center;
} Room;

typedef struct {
    Position pos;
    wchar_t *ch;
    int color;
    Direction direction;
    unsigned char visible;
    unsigned char seen;
    unsigned char alive;
} Entity;

typedef struct enemy_t {
    Position pos;
    wchar_t *ch;
    int color;
    Direction direction;
    Entity* target;
    unsigned char state;
    struct enemy_t* prev;
    struct enemy_t* next;
} Enemy;

// engine.c prototypes
int cursesSetup(void);
void gameLoop(void);
void closeGame(void);

// player.c prototypes
Entity *createPlayer(Position start_pos);
void handleInput(int input);
void movePlayer(Position newPos);
void interact();
void checkHealth();
void die();

// map.c prototypes
Tile** createMapTiles(void);
Position setupMap(void);
void freeMap(void);

// room.c prototypes
Room createRoom(int y, int x, int height, int width);
void addRoomToMap(Room room);
void connectRoomCenters(Position centerOne, Position centerTwo);

// draw.c prototypes
void drawMap(void);
void drawEntity(Entity* entity);
void drawEnemy(Enemy* enemy);
void drawPlayer(Entity* entity);
void drawEnemies(Enemy** enemies, int num);
void drawAll(void);
void firstPersonDrawMap(void);
void drawUI(void);

// fov.c prototypes
void makeFOV(Entity* player, int radius);
void clearFOV(Entity* player, int radius);
int getDistance(Position origin, Position target);
double getFloatDistance(Position origin, Position target);
int isInMap(int y, int x);
int lineOfSight(Position origin, Position target);
int getSign(int a);

// weapon.c prototypes
Entity *createProjectile();
void resetProjectile(Entity *proj);
void fireProjectile(Entity *proj, Entity *owner);
void moveProjectile(Entity *proj);

// audio.c prototypes
int init_audio();
void play_sound_effect(const char* filepath);
void play_string(char* string);
void play_char(char ch);
int load_music(const char* filepath);
int play_music();

// rand.c prototypes
int rand_lut();
int rand_range(int min, int max);

// enemy.c prototypes
Direction pick_direction();
void move_enemy(Enemy *enemy);
void move_enemies(Enemy **enemies, int num);
Enemy *create_enemy();
Enemy **create_enemy_array(int num);
void kill_enemy(Enemy* enemy);

// debug.c prototypes
void write_log(const char*);
void clear_log();

// npc.c prototypes
void spawn_dialog(char* string);

// externs
extern int MAP_HEIGHT;
extern int MAP_WIDTH;

extern int SCREEN_HEIGHT;
extern int SCREEN_WIDTH;

extern Entity*        player;
extern Entity*        projectile;
extern Enemy**        enemies;
extern Tile**         map;
extern unsigned char  canFire;
extern unsigned char  health;

extern WINDOW* ui;

#endif
