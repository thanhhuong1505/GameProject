#include "game.h"
#include "lib.h"
#include <time.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

const short int FPS = 60;
const short int frameDelay = 1000 / FPS;

using namespace std;

int main(int argc, char** argv)
{

    Uint32 frameStart;
    short int frameTime;
    game g;
    bool isMainMenu = true; // trạng thái menu chính
    bool isMenu = false;
    bool isPause = false;
    bool isSound = true;
    bool isDark = false;
    bool isCharacterSelection = false ; // bắt đầu từ menu chính, không phải chọn nhân vật
    bool isSoundSelection = false; // trạng thái chọn âm thanh
    
    while (!g.isQuit())
    {
        frameStart = SDL_GetTicks();
        int selectedCharacter = -1; // -1 là giá trị chưa chọn
        int selectedSoundSet = -1;  // -1 là giá trị chưa chọn

        if (isMainMenu)
        {
            g.takeInput();
            g.renderMainMenu();
            int selection = g.checkMainMenu();

            if (g.userInput.Type == game::input::PLAY)
            {
                if (selection == 0) // Play
                {
                    
                    isMainMenu = false; // không vào chọn nhân vật, vào game luôn
                    if (selectedCharacter != -1) {
                        g.shiba.init(selectedCharacter, isDark); // Dùng nhân vật đã chọn
                    }
                    if (selectedSoundSet != -1) {
                        g.sound.init(selectedSoundSet); // Dùng âm thanh đã chọn
                    }
                    g.userInput.Type = game::input::NONE;
                }
                else if (selection == 1) // Settings
                {
                    isMainMenu = false;
                    isCharacterSelection = true; // chuyển sang chọn nhân vật
                    g.userInput.Type = game::input::NONE;
                }
                else if (selection == 2) // Exit
                {
                    g.setQuit(true); // thoát game ngay lập tức
                }
            }
            
            g.display();
        }

        else if (isCharacterSelection)
        {
            g.takeInput();
            g.renderCharacterSelection(); // hiển thị màn hình chọn nhân vật
            g.renderBackButton();
            SDL_Color textColor = {255, 255, 255, 255}; // Màu trắng
            g.renderText("Select Your Character", 50, 50, textColor); // Hiển thị chữ hướng dẫn
            int selection = g.checkCharacterSelection();

            if (g.userInput.Type == game::input::PLAY)
            {
                if (g.checkBackButton()) {
                    isCharacterSelection = false;
                    isMainMenu = true; // Quay lại Main Menu
                    g.userInput.Type = game::input::NONE;
                } 
                else if (selection >= 0) {
                    selectedCharacter = selection; // Lưu nhân vật đã chọn
                    g.setSelectedCharacter(selectedCharacter);
                } 
                else if (selection == -1) {
                    isCharacterSelection = false;
                    isSoundSelection = true;
                    g.shiba.init(selectedCharacter, isDark);
                    g.userInput.Type = game::input::NONE;
                }
            }
            g.display();
        }
        else if (isSoundSelection){
            g.takeInput();
            g.renderSoundSelection(); // hiển thị màn hình chọn âm thanh
            g.renderBackButton();
            
            SDL_Color textColor = {255, 255, 255, 255}; // Màu trắng
            g.renderText("Select Sound Options", 50, 50, textColor); // Hiển thị chữ hướng dẫn
            int selection = g.checkSoundSelection();

            if(g.userInput.Type == game::input::PLAY){
                if (g.checkBackButton()) {
                    isSoundSelection = false;
                    isMainMenu = true; // Quay lại Main Menu
                    g.userInput.Type = game::input::NONE;
                } 
                else if (selection >= 0) {
                    selectedSoundSet = selection; // Lưu âm thanh đã chọn
                    g.setSelectedSoundset(selectedSoundSet);
                    g.sound.playBreath();
                } 
                else if (selection == -1) {
                    isSoundSelection = false;
                    g.sound.Free();
                    g.sound.init(g.getSelectedSoundSet());
                    g.userInput.Type = game::input::NONE;
                }
            }
            g.display();
        }
        else if (g.isDie())
        {
            if (isMenu) {
                g.sound.playHit();
                g.shiba.render();
            }
            g.userInput.Type = game::input::NONE;
            while (g.isDie() && !g.isQuit())
            {
                g.takeInput();
                if (isMenu && g.userInput.Type == game::input::PLAY)
                {
                    if (g.checkReplay())
                    {
                        
                        isMenu = false;
                    }
                    g.userInput.Type = game::input::NONE;
                }
                if (!isDark) g.renderBackground();
                else g.renderBackgroundNight();
                g.pipe.render();
                g.land.render();
                if (isMenu)
                {
                    g.shiba.render();
                    g.shiba.fall();
                    g.renderGameOver();
                    g.renderMedal();
                    g.renderScoreSmall();
                    g.renderBestScore();
                    g.replay();
                }
                else
                {
                    g.pipe.init();
                    g.shiba.init(g.getSelectedCharacter(), isDark); // Sử dụng getter
                    g.shiba.render();
                    g.renderMessage();
                    if (g.userInput.Type == game::input::PLAY)
                    {
                        g.Restart();
                        isMenu = true;
                        g.userInput.Type = game::input::NONE;
                    }
                    g.land.update();
                }
                g.display();
            }
            g.pipe.init();
        }
        else
        {
            g.takeInput();

            if (g.userInput.Type == game::input::PAUSE)
            {
                isPause = abs(1 - isPause);
                g.userInput.Type = game::input::NONE;
            }

            if (isPause == 0 && g.userInput.Type == game::input::PLAY)
            {
                if (isSound) g.sound.playBreath();
                g.shiba.resetTime();
                g.userInput.Type = game::input::NONE;
            }

            if (!g.getIsDark()) g.renderBackground();
            else g.renderBackgroundNight();
            g.pipe.render();
            g.land.render();
            g.shiba.render();
            g.renderScoreLarge();

            if (!isPause)
            {
                g.shiba.update(g.getPipeWidth(), g.getPipeHeight());
                g.pipe.update();
                g.land.update();
                
                g.pause();
            }
            else
            {
                g.resume();
                g.renderPauseTab();
                g.renderScoreSmall();
                g.renderBestScore();
                g.replay();
                g.sound.renderSound();
                if (!g.getIsDark()) g.lightTheme(); else g.darkTheme();
                g.nextButton();
                if (g.userInput.Type == game::input::PLAY)
                {
                    if (g.checkReplay())
                    {
                        
                        isPause = false;
                    }
                    else if (g.sound.checkSound())
                    {
                        isSound = abs(1 - isSound);
                    }
                    else if (g.changeTheme())
                    {
                        g.setIsDark(abs(1 - g.getIsDark())); 
                        g.shiba.init(g.getSelectedCharacter(), g.getIsDark()); // Sử dụng getter
                    }
                    g.userInput.Type = game::input::NONE;
                }
            }
            
            g.display();
        }

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    return 0;
}