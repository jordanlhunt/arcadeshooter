#include "main.h"
static void capFrameRate(long *currentTicks, float *remainder);
int main(int argc, char *argv[])
{
    long currentTicks;
    float remainder;
    memset(&app, 0, sizeof(App));
    app.textureTail = &app.textureHead;
    initSDL();
    atexit(shutDown);
    initGame();
    initTitle();
    currentTicks = SDL_GetTicks();
    remainder = 0;
    // Main game loop
    while (1)
    {
        prepareScene();
        handleInput();
        app.gameManager.logic();
        app.gameManager.draw();
        presentScene();
        capFrameRate(&currentTicks, &remainder);
    }
    return 0;
}
static void capFrameRate(long *currentTicks, float *remainder)
{
    long delay;
    long frameTime;
    delay = 16 + *remainder;
    *remainder -= (int)*remainder;
    frameTime = SDL_GetTicks() - *currentTicks;
    delay -= frameTime;
    if (delay < 1)
    {
        delay = 1;
    }
    SDL_Delay(delay);
    *remainder += 0.667;
    *currentTicks = SDL_GetTicks();
}