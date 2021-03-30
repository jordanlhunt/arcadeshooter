#include "common.h"
extern App app;

extern SDL_Texture *loadTexture(char *fileName);
extern void blitRectangle(SDL_Texture *texture, SDL_Rect *src, int x, int y);
extern void drawBackground();
extern void drawStarfield();
extern void drawText(int x, int y, int r, int g, int b, int align, char *format, ...);
extern void handleBackground();
extern void handleStarfield();
extern void initHighScores();
extern void initStage();
