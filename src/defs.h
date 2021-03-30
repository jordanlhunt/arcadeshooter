#define ENEMY_BULLET_SPEED 8
#define FONT_SIZE 22
#define FPS 60
#define MAX_FONT_LINE_LENGTH 1024
#define MAX_KEYBOARD_KEYS 350
#define MAX_SOUND_CHANNELS 8
#define MAX_STARS 500
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define NUMBER_OF_HIGH_SCORES 8
#define PLAYER_HEIGHT 144
#define PLAYER_LASER_SPEED 20
#define PLAYER_SPEED 4
#define PLAYER_WIDTH 144
#define SCREEN_HEIGHT 720
#define SCREEN_WIDTH 1280
#define SOURCE_ENEMY 1
#define SOURCE_PLAYER 0
#define STRINGCOPY(destination, source, n) \
    strncpy(destination, source, n);       \
    destination[n - 1] = '\0'
#define MAX_NAME_LENGTH 32
enum
{
    CHANNEL_ANY = -1,
    CHANNEL_PLAYER,
    CHANNEL_ALIEN_FIRE,
    CHANNEL_POINT_PICK_UP
};
enum
{
    SOUND_PLAYER_FIRE,
    SOUND_PLAYER_DIE,
    SOUND_PLAYER_IS_HIT,
    SOUND_ALIEN_FIRE,
    SOUND_ALIEN_DIE,
    SOUND_ALIEN_IS_HIT,
    SOUND_POINT_PICK_UP,
    SOUND_MAX
};
enum
{
    TEXT_LEFT,
    TEXT_CENTER,
    TEXT_RIGHT
};