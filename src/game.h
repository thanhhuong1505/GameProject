#pragma once

#include "doge.h"
#include "pipe.h"
#include "land.h"
#include "sound.h"
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class game : LTexture
{
public:
    struct input
    {
        enum type { QUIT, PLAY, NONE, PAUSE };
        type Type;
    };
    input userInput;
    doge shiba;
    pipe pipe;
    sound sound;
    land land;
    

public:
    game();
    ~game();

    bool initGraphic();
    bool isQuit() { return quit; }
    bool isDie() { return die; }
    void setQuit(bool value) { quit = value;} // thêm setter cho quit

    int getPipeWidth() { return pipe.width(); }
    int getPipeHeight() { return pipe.height(); }

    void takeInput();
    void display();
    void releaseGraphic();

    void renderScoreSmall();
    void renderScoreLarge();
    void renderBestScore();
    void renderMessage();
    void renderBackground();
    void renderBackgroundNight();
    void renderLand();
    void resume();
    void pause();
    void renderPauseTab();
    void lightTheme();
    void darkTheme();
    void nextButton();
    bool changeTheme();
    void renderGameOver();
    void renderMedal();
    void replay();
    bool checkReplay();
    void Restart();

    // Thêm các hàm cho menu chọn nhân vật
    void renderCharacterSelection();
    int checkCharacterSelection();
    
    // các hàm cho menu chọn âm thanh
    void renderSoundSelection();
    int checkSoundSelection();

    // các hàm cho menu chính
    void renderMainMenu();
    int checkMainMenu(); // trả về: 0(play), 1(settings) , 2(exit), -1 (không chọn)
    
    // hàm tiện ích để render văn bản
    void renderText(const char* text, int x, int y, SDL_Color color);

    // Thêm getter và setter cho selectedCharacter
    int getSelectedCharacter() const { return selectedCharacter; }
    void setSelectedCharacter(int character) { selectedCharacter = character; }
    
    // getter và setter cho selectedSoundSet
    int getSelectedSoundSet() const { return selectedSoundSet; }
    void setSelectedSoundset(int soundSet) { selectedSoundSet = soundSet;}

    // getter và setter cho isDark
    bool getIsDark() const { return isDark;}
    void setIsDark(bool dark) { isDark = dark;}
private:
    const double scaleNumberS = 0.75;
    short int bestScore;
    int selectedCharacter = 0; // Nhân vật được chọn, mặc định là Shiba
    int selectedSoundSet = 0; // Bộ âm thanh được chọn, mặc định là Shiba
    bool isDark = false; // trạng thái chủ đề, mặc định là sáng
    bool quit = false; // biến để kiểm tra thoát trò chơi
    
    // biến để lưu tọa độ các nút trong main menu
    SDL_Rect playButtonRect;
    SDL_Rect settingsButtonRect;
    SDL_Rect exitButtonRect;

    
};