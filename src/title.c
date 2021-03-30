#include "title.h"
static void logic();
static void draw();
static void drawLogo();
static SDL_Texture *logoTexture;
static int reveal = 0;
static int timeOut;
void initTitle()
{
    app.gameManager.logic = logic;
    app.gameManager.draw = draw;
    memset(app.keyboard, 0, (sizeof(int) * MAX_KEYBOARD_KEYS));
    logoTexture = loadTexture("images/logo.png");
    timeOut = FPS * 5;
}
static void logic()
{
    handleBackground();
    handleStarfield();
    if (reveal < SCREEN_HEIGHT)
    {
        reveal++;
    }
    if (timeOut-- == 0)
    {
        initHighScores();
    }
    if (app.keyboard[SDL_SCANCODE_J])
    {
        initStage();
    }
}
static void draw()
{
    drawBackground();
    drawStarfield();
    drawLogo();
    if (timeOut % 40 < 20)
    {
        drawText(SCREEN_WIDTH / 2, 600, 255, 255, 255, TEXT_CENTER, "PRESS FIRE TO PLAY");
    }
}
static void drawLogo(void)
{
    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    SDL_QueryTexture(logoTexture, NULL, NULL, &r.w, &r.h);
    r.h = MIN(reveal, r.h);
    blitRectangle(logoTexture, &r, (SCREEN_WIDTH / 2) - (r.w / 2), 100);
}