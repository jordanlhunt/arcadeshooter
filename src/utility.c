#include "utility.h"

int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
    return (MAX(x1, x2) < MIN(x1 + w1, x2 + w2)) && (MAX(y1, y2) < MIN(y1 + h1, y2 + h2));
}
void calculateSlope(int x1, int y1, int x2, int y2, float *directionX, float *directionY)
{
    int normalizedStep = MAX(abs(x1 - x2), abs(y1 - y2));
    if (normalizedStep == 0)
    {
        *directionX = 0;
        *directionY = 0;
        return;
    }
    *directionX = (x1 - x2);
    *directionX /= normalizedStep;
    *directionY = (y1 - y2);
    *directionY /= normalizedStep;
}