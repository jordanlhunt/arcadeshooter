#include "common.h"
extern App app;
extern GameStage gameStage;
extern Highscores highScores;
extern int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
extern SDL_Texture *loadTexture(char *fileName);
extern void addHighScore(int newHighScore);
extern void blit(SDL_Texture *texture, int x, int y);
extern void blitRectangle(SDL_Texture *texture, SDL_Rect *source, int x, int y);
extern void calculateSlope(int x1, int y1, int x2, int y2, float *directionX, float *directionY);
extern void drawText(int x, int y, int r, int g, int b, int align, char *format, ...);
extern void initHighScores();
extern void loadMusic(char *filename);
extern void playMusic(int loop);
extern void playSound(int soundId, int channel);
extern void handleBackground();
extern void handleStarfield();
extern void drawStarfield();
extern void drawBackground();