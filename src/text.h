#include "common.h"
#define GLYPH_HEIGHT 28
#define GLYPH_WIDTH 18

extern void blitRectangle(SDL_Texture *texture, SDL_Rect *src, int x, int y);
extern SDL_Texture *loadTexture(char *fileName);