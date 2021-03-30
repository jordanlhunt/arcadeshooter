#include "input.h"

// Handle key down for player actions
void handleKeyDown(SDL_KeyboardEvent *keyEvent)
{
    if (keyEvent->repeat == 0 && keyEvent->keysym.scancode < MAX_KEYBOARD_KEYS)
    {
        app.keyboard[keyEvent->keysym.scancode] = 1;
    }
}
// Handle key up for player action. Test to see if the event is a not a keyboard repeat
void handleKeyUp(SDL_KeyboardEvent *keyEvent)
{
    if (keyEvent->repeat == 0 && keyEvent->keysym.scancode < MAX_KEYBOARD_KEYS)
    {
        app.keyboard[keyEvent->keysym.scancode] = 0;
    }
}

// Handles user input
void handleInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            exit(0);
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            // If the user press ESC quit the game
            case SDLK_ESCAPE:
                exit(0);
                break;
            }
            handleKeyDown(&event.key);
            break;

        case SDL_KEYUP:
            handleKeyUp(&event.key);
            break;
        default:
            break;
        }
    }
}