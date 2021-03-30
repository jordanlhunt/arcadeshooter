#include "highscore.h"
static void logic();
static void draw();
static int compareHighScores(const void *score1, const void *score2);
static void drawHighScores();
static int timeOut;
void initHighScoreTable()
{
    int i;
    memset(&highScores, 0, sizeof(Highscores));
    for (i = 0; i < NUMBER_OF_HIGH_SCORES; i++)
    {
        highScores.highScoresArray[i].score = NUMBER_OF_HIGH_SCORES - i;
    }
}
void initHighScores()
{
    app.gameManager.logic = logic;
    app.gameManager.draw = draw;
    memset(app.keyboard, 0, (sizeof(int) * MAX_KEYBOARD_KEYS));
    timeOut = FPS * 5;
}

static void logic()
{
    handleBackground();
    handleStarfield();

    if (timeOut-- == 0)
    {
        initTitle();
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
    drawHighScores();
    if (timeOut % 40 < 20)
    {
        drawText(SCREEN_WIDTH / 2, 600, 255, 255, 255, TEXT_CENTER, "PRESS FIRE TO PLAY!");
    }
}
static void drawHighScores()
{
    int i;
    int yPosition;

    drawText(425, 70, 255, 255, 255, TEXT_CENTER, "HIGHSCORES");
    for (i = 0; i < NUMBER_OF_HIGH_SCORES; i++)
    {
        if (highScores.highScoresArray[i].recent)
        {
            drawText(425, yPosition, 255, 255, 0, TEXT_CENTER, "#%d .......... %03d", (i + 1), highScores.highScoresArray[i].score);
        }
        else
        {
            drawText(425, yPosition, 255, 255, 255, TEXT_CENTER, "#%d .......... %03d", (i + 1), highScores.highScoresArray[i].score);
        }
        yPosition += 50;
    }
    drawText(SCREEN_WIDTH / 2, 600, 255, 255, 255, TEXT_CENTER, "PRESS FIRE TO PLAY!");
}
// Add a highScore into the highScoreArray
void addHighScore(int newScore)
{
    Highscore newHighscores[NUMBER_OF_HIGH_SCORES + 1];
    int i;
    for (i = 0; i < NUMBER_OF_HIGH_SCORES; i++)
    {
        newHighscores[i] = highScores.highScoresArray[i];
        newHighscores[i].recent = 0;
    }
    newHighscores[NUMBER_OF_HIGH_SCORES].score = newScore;
    newHighscores[NUMBER_OF_HIGH_SCORES].recent = 1;
    qsort(newHighscores, NUMBER_OF_HIGH_SCORES + 1, sizeof(Highscore), compareHighScores);


    for (i = 0; i < NUMBER_OF_HIGH_SCORES; i++)
    {
        highScores.highScoresArray[i] = newHighscores[i];
    }
}

static int compareHighScores(const void *score1, const void *score2)
{
    Highscore *highscore1 = ((Highscore *)score1);
    Highscore *highscore2 = ((Highscore *)score2);
    return (highscore2->score - highscore1->score);
}