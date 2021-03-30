#include "init.h"
void initSDL()
{
    int rendererFlags = SDL_RENDERER_ACCELERATED;
    int windowFlags = 0;
    // Start up SDL2 video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("[FAILURE] - Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    else
    {
        printf("[SUCCESS] - SDL_VIDEO initialized\n");
    }
    // Start up SDL2 Image subsystem
    // Create the app window
    app.window = SDL_CreateWindow("Moby: Outlaw Pirate Of Liberty", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);
    if (app.window == NULL)
    {
        printf("[FAILURE] - Failed to open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
        exit(1);
    }
    else
    {
        printf("[SUCCESS] - App window created\n");
    }
    // Create main renderer
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);
    if (app.renderer == NULL)
    {
        printf("[FAILURE] - Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }
    else
    {
        printf("[SUCCESS] - SDL renderer initialized\n");
    }
    // Initialize SDL image subsystem
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0)
    {
        printf("[FAILURE] - Failed to initialize SDL_image! SDL error: %s\n", SDL_GetError());
        exit(1);
    }
    else
    {
        printf("[SUCCESS] - SDL_image initialized\n");
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
    {
        printf("[FAILURE] - Failed to initialize SDL_Mixer!");
        exit(1);
    }
    else
    {
        printf("[SUCCESS] - SDL_Mixer initialized\n");
        Mix_AllocateChannels(MAX_SOUND_CHANNELS);
    }
    SDL_ShowCursor(0);
}
void shutDown()
{
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    SDL_Quit();
}

// Calls all the game startup calls
void initGame()
{
    initBackground();
    initStarfield();
    initSounds();
    initFonts();
    initHighScores();
    loadMusic("music/the_last_parsec_1.2.ogg");
    playMusic(1);
}
