#include "draw.h"
void prepareScene()
{
    // Draw the background color
    SDL_SetRenderDrawColor(app.renderer, 0x69, 0x95, 0xED, 0xFF);
    SDL_RenderClear(app.renderer);
}
void presentScene()
{
    // Draw the components in the renderer
    SDL_RenderPresent(app.renderer);
}
static void addTextureToCache(char *fileName, SDL_Texture *sdlTexture)
{
    Texture *texture = malloc(sizeof(Texture));
    memset(texture, 0, sizeof(Texture));
    app.textureTail->next = texture;
    app.textureTail = texture;
    STRINGCOPY(texture->name, fileName, MAX_NAME_LENGTH);
    texture->texture = sdlTexture;
}
// Whenever a texture is loaded, check if it has already been loaded, if NOT then add it to the cache
static SDL_Texture *getTexture(char *fileName)
{
    Texture *textureToGet;
    for (textureToGet = app.textureHead.next; textureToGet != NULL; textureToGet = textureToGet->next)
    {
        if (strcmp(textureToGet->name, fileName) == 0)
        {
            return textureToGet->texture;
        }
    }
    return NULL;
}
// Loads an image and returns it as a texture
SDL_Texture *loadTexture(char *fileName)
{
    SDL_Texture *texture;
    texture = getTexture(fileName);
    if (texture == NULL)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[STANDBY] - Loading %s", fileName);
        texture = IMG_LoadTexture(app.renderer, fileName);
        addTextureToCache(fileName, texture);
    }
    return texture;
}
// Draws the a texture on screen at a specified X and Y
void blit(SDL_Texture *texture, int x, int y)
{
    // Destination Rectangle
    SDL_Rect destinationRectangle;
    destinationRectangle.x = x;
    destinationRectangle.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &destinationRectangle.w, &destinationRectangle.h);
    SDL_RenderCopy(app.renderer, texture, NULL, &destinationRectangle);
}
// Blit function that takes texture and coordinates, it also takes a rectangeular
void blitRectangle(SDL_Texture *texture, SDL_Rect *source, int x, int y)
{
    SDL_Rect destination;
    destination.x = x;
    destination.y = y;
    destination.w = source->w;
    destination.h = source->h;
    SDL_RenderCopy(app.renderer, texture, source, &destination);
}