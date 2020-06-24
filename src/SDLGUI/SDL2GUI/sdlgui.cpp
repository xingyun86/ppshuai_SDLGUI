// sdlgui.cpp : Defines the entry point for the application.
//

#include "sdlgui.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_net.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

using namespace std;

#ifdef main
#undef main
#endif // main

int main(int argc, char* argv[])
{
	cout << "Hello CMake." << endl;

	int ret = SDL_Init(SDL_INIT_EVERYTHING);
	if (ret != 0) {
		return -1;
	}
	SDL_Window* pWin = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE);
	if (pWin == NULL) {
		return -2;
	}
	SDL_Renderer* pRenderer = SDL_CreateRenderer(pWin, -1, SDL_RENDERER_ACCELERATED);
	if (pRenderer == NULL) {
		SDL_DestroyWindow(pWin);
		return -3;
	}

	int quit = 0;
	SDL_Event evt;

	SDLNet_Init();
	TTF_Init();
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);

	// open 44.1KHz, signed 16bit, system byte order,
	//      stereo audio, using 1024 byte chunks
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
		exit(2);
	}
	Mix_Music* music = Mix_LoadMUS("mp3/classic.mp3");
	Mix_Music* music1 = Mix_LoadMUS("mp3/luozi.mp3");
	Mix_VolumeMusic(30);
	Mix_PlayMusic(music, -1);
	//加载声效
	Mix_Chunk * chunk1 = Mix_LoadWAV("wav/Alarm01.wav");
	Mix_Chunk * chunk2 = Mix_LoadWAV("wav/Alarm02.wav");
	Mix_AllocateChannels(2);

	////////////////////////////////////////////////////////////////
	Uint64 nFrequency, nPrevCounter, nCurrCounter, nElapsedCounter;
	float elapsed = 0.0f, totalTime = 0.0f;
	int fps = 0, fpsCount = 0;

	nFrequency = SDL_GetPerformanceFrequency();
	nPrevCounter = SDL_GetPerformanceCounter();

	while (!quit) {
		if (SDL_PollEvent(&evt)) {
			if (evt.type == SDL_QUIT) {
				quit = 1;
			}
			else if (evt.type == SDL_KEYDOWN) {
				//键盘按下事件，可以查看 SDL_KeyboardEvent 结构体内容
				//evt.key.keysym
			}
			else if (evt.type == SDL_MOUSEBUTTONDOWN) {
				//鼠标按钮按下事件，可以查看 SDL_MouseButtonEvent 结构体内容
				//evt.button.x, evt.button.y
				//Mix_PlayMusic(music1, -1);
				Mix_PlayChannel(-1, chunk1, 0);
				Mix_PlayChannel(-1, chunk2, 0);
			}
		}
		else {
			//清除背景
			SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
			SDL_RenderClear(pRenderer);

			//TODO:渲染图像

			{
				SDL_Surface* bmp = SDL_LoadBMP("test.bmp");
				SDL_Texture* text = SDL_CreateTextureFromSurface(pRenderer, bmp);
				SDL_RenderCopy(pRenderer, text, NULL, NULL);
				SDL_FreeSurface(bmp);
				SDL_DestroyTexture(text);
			}
			{
				SDL_Surface* png = IMG_Load("test.png");
				SDL_Texture* text = SDL_CreateTextureFromSurface(pRenderer, png);
				SDL_RenderCopy(pRenderer, text, NULL, NULL);
				SDL_FreeSurface(png);
				SDL_DestroyTexture(text);
			}

			{
				SDL_Color color = { 255, 255, 255, 255 };
				TTF_Font* font = TTF_OpenFont("simsun.ttf", 9);
				SDL_Surface* surf = TTF_RenderText_Blended(font, "are you OK ?", color);
				SDL_Texture* ftex = SDL_CreateTextureFromSurface(pRenderer, surf);
				SDL_RenderCopy(pRenderer, ftex, NULL, NULL);
				SDL_FreeSurface(surf);
				SDL_DestroyTexture(ftex);
			}
			{
				nCurrCounter = SDL_GetPerformanceCounter();
				nElapsedCounter = nCurrCounter - nPrevCounter;
				nPrevCounter = nCurrCounter;

				//前后两帧的耗时(ms)
				elapsed = (nElapsedCounter * 1000.0f) / nFrequency;

				//清除背景
				SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
				SDL_RenderClear(pRenderer);

				//TODO: 绘制其它

				//显示图像
				SDL_RenderPresent(pRenderer);


				totalTime += elapsed;
				//已经过了 1 秒
				if (totalTime > 1000.0f) {
					totalTime -= 1000.0f;
					fps = fpsCount;
					fpsCount = 1;

					SDL_Log("%d\n", fps);

				}
				else {
					fpsCount++;
				}
			}
			//显示图像
			SDL_RenderPresent(pRenderer);

		}
	}

	Mix_FreeMusic(music);
	Mix_FreeMusic(music1);

	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDLNet_Quit();

	if (pRenderer) {
		SDL_DestroyRenderer(pRenderer);
	}


	if (pWin) {
		SDL_DestroyWindow(pWin);
	}
	SDL_Quit();

	return 0;
}
