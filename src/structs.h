typedef struct Entity Entity;
typedef struct Debris Debris;
typedef struct Explosion Explosion;
typedef struct Texture Texture;
// The GameManager will be the manager game related function in the main game loop
typedef struct
{
    void (*logic)();
    void (*draw)();
} GameManager;
struct Texture
{
    char name[MAX_NAME_LENGTH];
    SDL_Texture *texture;
    Texture *next;
};
typedef struct
{
    SDL_Renderer *renderer;
    SDL_Window *window;
    GameManager gameManager;
    // Holds all the possible states of the keyboard
    int keyboard[MAX_KEYBOARD_KEYS];
    // A linked list of all loaded Textures
    Texture textureHead;
    Texture *textureTail;
} App;
struct Entity
{
    Entity *next;
    float directionX;
    float directionY;
    int hitPoints;
    int reload;
    int side;
    int h;
    int w;
    int x;
    int y;
    SDL_Texture *texture;
};
// Hold the details of the explosion of a ship
struct Explosion
{
    Explosion *next;
    float directionX;
    float directionY;
    float x;
    float y;
    int alpha;
    int blue;
    int green;
    int red;
};
// Hold the details of the debris of a ship after an explosion
struct Debris
{
    Debris *next;
    float directionX;
    float directionY;
    float x;
    float y;
    int lifespan;
    SDL_Rect rectangle;
    SDL_Texture *texture;
};
typedef struct
{
    int x;
    int y;
    int brightness;
} Star;
typedef struct
{
    Debris *debrisTail;
    Debris debrisHead;
    Entity *laserTail;
    Entity *pointPickUpTail;
    Entity *shipTail;
    Entity laserHead;
    Entity pointPickUpHead;
    Entity shipHead;
    Explosion *explosionTail;
    Explosion explosionHead;
    int score;
} GameStage;
// High score object
typedef struct
{
    int recent;
    int score;
} Highscore;
typedef struct
{
    Highscore highScoresArray[NUMBER_OF_HIGH_SCORES];
} Highscores;
