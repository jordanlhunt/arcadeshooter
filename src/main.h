#include "common.h"

extern void handleInput();
extern void initSDL();
extern void initGame();
extern void initTitle();
extern void prepareScene();
extern void presentScene();
extern void shutDown();

App app;
GameStage gameStage;
Highscores highScores;