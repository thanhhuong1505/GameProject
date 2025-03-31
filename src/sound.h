#pragma once

#include <SDL2/SDL_mixer.h>
#include <string>
#include "lib.h"

using namespace std;

class sound:LTexture
{
public:
    bool init(int soundSetId); // thêm tham số soundSetId

    void Free();

    void playBreath();

    void playHit();

    void renderSound();

    bool checkSound();

private:
    const short int POS_X = 107;
    const short int POS_Y = 267;
    bool isPlay = true; // bật tắt âm thanh
    int currentSoundSet; // bộ âm thanh hiện tại
    Mix_Chunk* breath = NULL;
    Mix_Chunk* hit = NULL;
    Mix_Chunk* drop = NULL;
    SDL_Rect Mute ;
    SDL_Rect Active ;
};
