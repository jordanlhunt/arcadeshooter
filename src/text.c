#include "text.h"
static SDL_Texture *scoreTextTexture;
static char drawTextBuffer[MAX_FONT_LINE_LENGTH];
void initFonts()
{
    scoreTextTexture = loadTexture("fonts/font.png");
}
void drawText(int x, int y, int red, int green, int blue, int align, char *format, ...)
{
    int i;
    int length;
    int textString;
    SDL_Rect rectangle;
    va_list args;
    memset(&drawTextBuffer, '\0', sizeof(drawTextBuffer));
    va_start(args, format);
    vsprintf(drawTextBuffer, format, args);
    va_end(args);
    length = strlen(drawTextBuffer);
    switch (align)
    {
    case TEXT_RIGHT:
        x -= (length * GLYPH_WIDTH);
        break;

    case TEXT_CENTER:
        x -= (length * GLYPH_WIDTH) / 2;
        break;
    }
    rectangle.y = 0;
    rectangle.w = GLYPH_WIDTH;
    rectangle.h = GLYPH_HEIGHT;
    SDL_SetTextureColorMod(scoreTextTexture, red, green, blue);
    for (i = 0; i < length; i++)
    {
        textString = drawTextBuffer[i];
        if (textString >= ' ' && textString <= 'Z')
        {
            rectangle.x = (textString - ' ') * GLYPH_WIDTH;
            blitRectangle(scoreTextTexture, &rectangle, x, y);
            x += GLYPH_WIDTH;
        }
    }
}