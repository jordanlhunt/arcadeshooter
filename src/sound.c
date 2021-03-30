#include "sound.h"
static void loadSounds();
static Mix_Chunk *sounds[SOUND_MAX];
static Mix_Music *music;
void initSounds()
{
    memset(sounds, 0, sizeof(Mix_Chunk *) * SOUND_MAX);
    music = NULL;
    loadSounds();
}
static void loadSounds()
{
    sounds[SOUND_ALIEN_DIE] = Mix_LoadWAV("sounds/alienLaser.ogg");
    sounds[SOUND_ALIEN_FIRE] = Mix_LoadWAV("sounds/alienLaser.ogg");
    sounds[SOUND_ALIEN_IS_HIT] = Mix_LoadWAV("sounds/laserHitsAlien.ogg");
    sounds[SOUND_PLAYER_DIE] = Mix_LoadWAV("sounds/playerDie.ogg");
    sounds[SOUND_PLAYER_FIRE] = Mix_LoadWAV("sounds/playerLaser.ogg");
    sounds[SOUND_PLAYER_IS_HIT] = Mix_LoadWAV("sounds/laserHitsplayer.ogg");
    sounds[SOUND_POINT_PICK_UP] = Mix_LoadWAV("sounds/score.ogg");
}
// Load the music file
void loadMusic(char *fileName)
{
    // If there is data in music, free it and load in he new data
    if (music != NULL)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
        music = NULL;
    }
    music = Mix_LoadMUS(fileName);
}
void playMusic(int loop)
{
    // Play the music file
    Mix_PlayMusic(music, (loop) ? -1 : 0);
}
void playSound(int soundId, int channel)
{
    // Play the sound from the sound array with the soundId
    Mix_PlayChannel(channel, sounds[soundId], 0);
}