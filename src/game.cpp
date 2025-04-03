#include "game.h"
#include "lib.h"
#include <iostream>

void game::takeInput()
{
    SDL_Event e;
    userInput.Type = input::NONE;

    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            userInput.Type = input::QUIT;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_SPACE:
                userInput.Type = input::PLAY;
                break;
            case SDLK_ESCAPE:
                userInput.Type = input::PAUSE;
                break;
            }
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                userInput.Type = input::PLAY;
            }
        }
    }

    // Nếu userInput.Type là QUIT, đặt quit = true
    if (userInput.Type == input::QUIT)
    {
        quit = true;
    }
}
game::game()
{
    initGraphic();
    pipe.init();
    land.init();
    sound.init(selectedSoundSet); // khỏi tạo với bộ âm thanh mặc định
	shiba.init(selectedCharacter, false); // khởi tạo với nhân vật mặc định

}

game::~game()
{
    shiba.Free();
    pipe.Free();
    land.Free();
    sound.Free();
    free();
    releaseGraphic();
}

void game::releaseGraphic()
{
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    SDL_DestroyRenderer( gRenderer );
    gRenderer = NULL;
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

bool game::initGraphic()
{
	bool success = true;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		gWindow = SDL_CreateWindow( "Flappy Doge", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

void game::display()
{
    SDL_RenderPresent(gRenderer);
    SDL_RenderClear(gRenderer);
}

void game::renderScoreSmall()
{
	string s = to_string(score);
	signed char len = s.length();
	LTexture image;

	for (signed char i = len - 1; i >= 0; i--)
	{
		signed char number = s[i] - '0';
		if (number == 1)
		{
			image.Load("data/small/1.png", scaleNumberS);
		}
		else if (number == 2)
		{
			image.Load("data/small/2.png", scaleNumberS);
		}
		else if (number == 3)
		{
			image.Load("data/small/3.png", scaleNumberS);
		}
		else if (number == 4)
		{
			image.Load("data/small/4.png", scaleNumberS);
		}
		else if (number == 5)
		{
			image.Load("data/small/5.png", scaleNumberS);
		}
		else if (number == 6)
		{
			image.Load("data/small/6.png", scaleNumberS);
		}
		else if (number == 7)
		{
			image.Load("data/small/7.png", scaleNumberS);
		}
		else if (number == 8)
		{
			image.Load("data/small/8.png", scaleNumberS);
		}
		else if (number == 9)
		{
			image.Load("data/small/9.png", scaleNumberS);
		}
		else
		{
			image.Load("data/small/0.png", scaleNumberS);
		}
		image.Render(260 - image.getWidth() * (len - i - 1) * 0.75 - 5 * (len - i - 1), 268);
	}
	image.free();
}

void game::renderScoreLarge()
{
    string s = to_string(score);
	signed char len = s.length();
	LTexture image;

	for (signed char i = 0; i < len; i++)
	{
		signed char number = s[i] - '0';
		if (number == 1)
		{
			image.Load("data/large/1.png", 1);
		}
		else if (number == 2)
		{
			image.Load("data/large/2.png", 1);
		}
		else if (number == 3)
		{
			image.Load("data/large/3.png", 1);
		}
		else if (number == 4)
		{
			image.Load("data/large/4.png", 1);
		}
		else if (number == 5)
		{
			image.Load("data/large/5.png", 1);
		}
		else if (number == 6)
		{
			image.Load("data/large/6.png", 1);
		}
		else if (number == 7)
		{
			image.Load("data/large/7.png", 1);
		}
		else if (number == 8)
		{
			image.Load("data/large/8.png", 1);
		}
		else if (number == 9)
		{
			image.Load("data/large/9.png", 1);
		}
		else
		{
			image.Load("data/large/0.png", 1);
		}
		image.Render((SCREEN_WIDTH - (image.getWidth() * len + (len - 1) * 10)) / 2 + (i + 30) * i, 100);
	}
	image.free();
}

void game::renderBestScore()
{
	ifstream fileIn("data/bestScore.txt");

	fileIn >> bestScore;
	ofstream fileOut("data/bestScore.txt", ios::trunc);

	if (score > bestScore)
	{
		bestScore = score;
	}
	string s = to_string(bestScore);
	signed char len = s.length();
	LTexture image;

	for (signed char i = len-1; i >= 0; i--)
	{
		signed char number = s[i] - '0';
		if (number == 1)
		{
			image.Load("data/small/1.png", scaleNumberS);
		}
		else if (number == 2)
		{
			image.Load("data/small/2.png", scaleNumberS);
		}
		else if (number == 3)
		{
			image.Load("data/small/3.png", scaleNumberS);
		}
		else if (number == 4)
		{
			image.Load("data/small/4.png", scaleNumberS);
		}
		else if (number == 5)
		{
			image.Load("data/small/5.png", scaleNumberS);
		}
		else if (number == 6)
		{
			image.Load("data/small/6.png", scaleNumberS);
		}
		else if (number == 7)
		{
			image.Load("data/small/7.png", scaleNumberS);
		}
		else if (number == 8)
		{
			image.Load("data/small/8.png", scaleNumberS);
		}
		else if (number == 9)
		{
			image.Load("data/small/9.png", scaleNumberS);
		}
		else
		{
			image.Load("data/small/0.png", scaleNumberS);
		}
		image.Render(260 - image.getWidth()*(len-i-1)*0.75 - 5*(len - i - 1), 315);
	}
	image.free();

	fileOut << bestScore;
	fileIn.close();
	fileOut.close();
}

void game::renderMessage()
{
	LTexture image;
	image.Load("data/image/message.png", 1);
	image.Render((SCREEN_WIDTH - image.getWidth()) / 2, 180);
	image.free();
}

void game::renderBackground()
{
	LTexture image;
	image.Load("data/image/background.png", 1);
	image.Render(0, 0);
	image.free();
}

void game::renderBackgroundNight()
{
	LTexture image;
	image.Load("data/image/background-night.png", 1);
	image.Render(0, 0);
	image.free();
}

void game::renderLand()
{
	LTexture image;
	image.Load("data/image/land.png", 1);
	image.Render((SCREEN_WIDTH - image.getWidth()) / 2, SCREEN_HEIGHT- image.getHeight());
	image.free();
}

void game::resume()
{
	LTexture image;
	image.Load("data/image/resume.png", 1);
	image.Render(SCREEN_WIDTH - 50, 20);
	image.free();
}

void game::pause()
{
	LTexture image;
	image.Load("data/image/pause.png", 1);
	image.Render(SCREEN_WIDTH - 50, 20);
	image.free();
}

void game::renderPauseTab()
{
	LTexture image;
	image.Load("data/image/pauseTab.png", 1);
	image.Render((SCREEN_WIDTH - image.getWidth()) / 2, 230);
	image.free();
}

void game::lightTheme()
{
	LTexture image;
	image.Load("data/image/shiba.png", 0.8);
	image.Render(105, 315);
	image.free();
}

void game::darkTheme()
{
	LTexture image;
	image.Load("data/image/shiba-dark.png", 0.8);
	image.Render(105, 315);
	image.free();
}

void game::nextButton()
{
	LTexture image;
	image.Load("data/image/nextRight.png", 1);
	image.Render(149, 322);
	image.Load("data/image/nextLeft.png", 1);
	image.Render(88, 322);
	image.free();
}

bool game::changeTheme()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	if (((x > 149 && x < 149+13)  || (x > 88 && x < 88 + 13)) && (y > 322 && y < 322 + 16))
	{
		return true;
	}
	return false;
}

void game::renderGameOver()
{
	LTexture image;
	image.Load("data/image/gameOver.png", 1);
	image.Render((SCREEN_WIDTH - image.getWidth()) / 2, 150);
	image.free();
}

void game::renderMedal()
{
	LTexture image;

	if (score > 20 && score <= 50)
	{
		image.Load("data/medal/silver.png", scaleNumberS);
	}
	else if (score > 50)
	{
		image.Load("data/medal/gold.png", scaleNumberS);
	}
	else
	{
		image.Load("data/medal/honor.png", scaleNumberS);
	}
	image.Render(82, 275);

	image.free();
}

void game::replay()
{
	LTexture image;
	image.Load("data/image/replay.png", 1);
	image.Render((SCREEN_WIDTH - image.getWidth()) / 2, 380);
	image.free();
}

bool game::checkReplay()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	if (x > (SCREEN_WIDTH - 100)/2 && x < (SCREEN_WIDTH + 100) / 2 && y > 380 && y < 380 + 60)
	{
		return true;
	}
	return false;
}

void game::Restart()
{
    die = false;
    score = 0;
    shiba.resetTime();
}

void game::renderCharacterSelection()
{
	//hiển thị nền

	if(!isDark) renderBackground();
	else renderBackgroundNight();
	land.render();
	// hiển thị các nhân vật để chọn
	LTexture shibaTex, pinkbirdTex, yellowbirdTex;
	shibaTex.Load("data/image/shiba.png", 0.8);
	pinkbirdTex.Load("data/image/pinkbird.png", 0.8);
	yellowbirdTex.Load("data/image/yellowbird.png", 0.8);

	shibaTex.Render(50, 200); // vị trí nhân vật shiba
	pinkbirdTex.Render(150, 200);
	yellowbirdTex.Render(250, 200);

	// đánh dấu nhân vật được chọn
	LTexture border;
	border.Load("data/image/selected_border.png", 1); // tạo một hình ảnh viền để đánh dấu
	if (selectedCharacter == 0) border.Render(50, 200);
    else if (selectedCharacter == 1) border.Render(150, 200);
    else if (selectedCharacter == 2) border.Render(250, 200);

    // Hiển thị nút "Start"
    LTexture startButton;
    startButton.Load("data/image/replay.png", 1); // Tái sử dụng nút replay làm nút Start
    startButton.Render((SCREEN_WIDTH - startButton.getWidth()) / 2, 400);

    // Giải phóng tài nguyên
    shibaTex.free();
    pinkbirdTex.free();
    yellowbirdTex.free();
    border.free();
    startButton.free();
}
int game::checkCharacterSelection()
{
    int x, y;
    SDL_GetMouseState(&x, &y);

    // Kiểm tra lựa chọn nhân vật (giả sử kích thước nhân vật là 50x35, thu nhỏ còn 80%)
    if (x > 50 && x < 50 + 40 && y > 200 && y < 200 + 28) return 0;   // Shiba
    if (x > 150 && x < 150 + 40 && y > 200 && y < 200 + 28) return 1; // Pinkbird
    if (x > 250 && x < 250 + 40 && y > 200 && y < 200 + 28) return 2; // Yellowbird

    // Kiểm tra nút Start
    if (x > (SCREEN_WIDTH - 100) / 2 && x < (SCREEN_WIDTH + 100) / 2 && y > 400 && y < 400 + 60)
        return -1; // Bắt đầu trò chơi

    return selectedCharacter; // Giữ nguyên lựa chọn hiện tại
}

void game::renderSoundSelection()
{
	// Hiển thị nền
	if(!isDark) renderBackground();
	else renderBackgroundNight();
    land.render();
	
	// Hiển thị các biểu tượng đại diện cho bộ âm thanh (dùng hình ảnh nhân vật để minh họa)
	LTexture shibaTex, pinkbirdTex, yellowbirdTex;
	shibaTex.Load("data/image/shiba.png", 0.8);
	pinkbirdTex.Load("data/image/pinkbird.png", 0.8);
	yellowbirdTex.Load("data/image/yellowbird.png", 0.8);

	//vị trí các biểu tượng âm thanh
	shibaTex.Render(50,200); // shiba sound set
	pinkbirdTex.Render(150, 200); // pinkbird sound set
    yellowbirdTex.Render(250, 200); // yellowbird sound set

	// đánh dấu bộ âm thanh được chọn bằng viền
	LTexture border;
	border.Load("data/image/selected_border.png", 1);
	if(selectedSoundSet == 0) border.Render(50, 200);
	else if(selectedSoundSet == 1) border.Render(150, 200);
	else if(selectedSoundSet == 2) border.Render(250, 200);

	// Hiển thị nút "Start"
	LTexture startButton;
	startButton.Load("data/image/replay.png", 1);
	startButton.Render((SCREEN_WIDTH - startButton.getWidth()) / 2, 400);

	// giải phóng tài nguyên
	shibaTex.free();
	pinkbirdTex.free();
	yellowbirdTex.free();
	border.free();
	startButton.free();

}

int game::checkSoundSelection()
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	// kiểm tra lựa chọn bộ âm thanh(kích thước biểu tượng 50x30, thu nhỏ còn 80%);
	if(x > 50 && x < 50 + 40 && y > 200 && y < 200 + 28) return 0; // shiba sound set
	if(x > 150 && x < 150 + 40 && y > 200 && y < 200 + 28) return 1; // pinkbird sound set
	if(x > 250 && x < 250 + 40 && y > 200 && y < 200 + 28) return 2; // yellowbird sound set

	// kiểm tra nút Start
	if(x > (SCREEN_WIDTH - 100) / 2 && x < (SCREEN_WIDTH + 100) / 2 && y > 400 && y < 400 + 60)
	  return -1; // bắt đầu trò chơi

	return selectedSoundSet; // giữ nguyên lựa chọn hiện tại  

}

void game::renderMainMenu()
{
    // Hiển thị nền
    LTexture background;
    background.Load("data/image/main_menu_background.png", 1);
    background.Render(0, 0);

    // Hiển thị các nút
    LTexture playButton, settingsButton, exitButton;
    playButton.Load("data/image/play_button.png", 1);
    settingsButton.Load("data/image/settings_button.png", 1);
    exitButton.Load("data/image/exit_button.png", 1);

    // Đặt vị trí các nút
    playButton.Render((SCREEN_WIDTH - playButton.getWidth()) / 2, 200);     // Nút Play: (125, 200)
    settingsButton.Render((SCREEN_WIDTH - settingsButton.getWidth()) / 2, 300); // Nút Settings: (125, 300)
    exitButton.Render((SCREEN_WIDTH - exitButton.getWidth()) / 2, 400);     // Nút Exit: (125, 400)

    // Giải phóng tài nguyên
    background.free();
    playButton.free();
    settingsButton.free();
    exitButton.free();
}

int game::checkMainMenu()
{
    int x, y;
    SDL_GetMouseState(&x, &y);

    // Kiểm tra nhấp vào nút Play (kích thước 100x50)
    if (x > 125 && x < 225 && y > 200 && y < 250) return 0; // Play

    // Kiểm tra nhấp vào nút Settings
    if (x > 125 && x < 225 && y > 300 && y < 350) return 1; // Settings

    // Kiểm tra nhấp vào nút Exit
    if (x > 125 && x < 225 && y > 400 && y < 450) return 2; // Exit

    return -1; // Không chọn

}
void game::renderText(const char* text, int x, int y, SDL_Color color)
{
    TTF_Font* font = TTF_OpenFont("data/font/arial.ttf", 24); // Đảm bảo có font trong thư mục data/font/
    if (font == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    if (surface == NULL)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);
    if (texture == NULL)
    {
        printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
        return;
    }

    int textWidth = surface->w;
    int textHeight = surface->h;
    SDL_Rect renderQuad = { x, y, textWidth, textHeight };
    SDL_RenderCopy(gRenderer, texture, NULL, &renderQuad);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font); // Đóng font để giải phóng tài nguyên
}

void game::renderBackButton() {
    SDL_Rect backButton = {100, 520, 100, 50}; // Vị trí và kích thước nút Back
    

    SDL_Color textColor = {255, 255, 255, 255}; // Màu trắng
    renderText("Back", 145, 540, textColor); // Hiển thị chữ "Back"
}

bool game::checkBackButton() {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    SDL_Rect backButton = {100, 520, 100, 50}; // Vị trí nút "Back"
    if (mouseX >= backButton.x && mouseX <= backButton.x + backButton.w &&
        mouseY >= backButton.y && mouseY <= backButton.y + backButton.h &&
        userInput.Type == input::PLAY) 
    {
        return true;
    }
    return false;
}