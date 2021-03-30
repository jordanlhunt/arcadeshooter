#include "background.h"

static int backgroundX;
static Star starField[MAX_STARS];
static SDL_Texture *backgroundTexture;

void initBackground()
{
    backgroundTexture = loadTexture("images/background.png");
    backgroundX = 0;
}
// Create the starfield, randomly place star on the screen
void initStarfield()
{
    int i;
    for (i = 0; i < MAX_STARS; i++)
    {
        starField[i].x = rand() % SCREEN_WIDTH;
        starField[i].y = rand() % SCREEN_HEIGHT;
        starField[i].brightness = 1 + rand() % 8;
    }
}
// Draw the background image of the cosmos
void drawBackground()
{
    SDL_Rect destination;
    int x;
    for (x = backgroundX; x < SCREEN_WIDTH; x += SCREEN_WIDTH)
    {
        destination.x = x;
        destination.y = 0;
        destination.w = SCREEN_WIDTH;
        destination.h = SCREEN_HEIGHT;
        SDL_RenderCopy(app.renderer, backgroundTexture, NULL, &destination);
    }
}
// Loop through the stars array and draw theme
void drawStarfield()
{
    int i;
    int star;
    for (i = 0; i < MAX_STARS; i++)
    {
        star = 32 * starField[i].brightness;
        // set the color of the star
        SDL_SetRenderDrawColor(app.renderer, star, star, star, 255);
        SDL_RenderDrawLine(app.renderer, starField[i].x, starField[i].y, starField[i].x + 3, starField[i].y);
    }
}
// This will handle the wrap around background in a parallax fashion
void handleBackground()
{
    if (backgroundX-- < -SCREEN_WIDTH)
    {
        backgroundX = 0;
    }
}
// Populate the screen with a starfield
void handleStarfield()
{
    int star;
    for (star = 0; star < MAX_STARS; star++)
    {
        starField[star].x -= starField[star].brightness;
        if (starField[star].x < 0)
        {
            starField[star].x = SCREEN_WIDTH + starField[star].x;
        }
    }
}
