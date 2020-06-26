// sdlgui.cpp : Defines the entry point for the application.
//

#include "sdlgui.h"

#include <thread>

#include <SDL.h>
#include <SDL_gif.h>
#include <SDL_ttf.h>
#include <SDL_net.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

using namespace std;

#define RES_PREFIX "res/"

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
	Uint32 tickStart = 0;
	Uint32 tickEnd = 0;

	////////////////////////////////////////////////////////////////
	Uint64 nFrequency, nPrevCounter, nCurrCounter, nElapsedCounter;
	float elapsed = 0.0f, totalTime = 0.0f;
	int fps = 0, fpsCount = 0;

	nFrequency = SDL_GetPerformanceFrequency();
	nPrevCounter = SDL_GetPerformanceCounter();

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
	Mix_Music* music = Mix_LoadMUS(RES_PREFIX "mp3/classic.mp3");
	Mix_Music* music1 = Mix_LoadMUS(RES_PREFIX "mp3/luozi.mp3");
	Mix_VolumeMusic(30);
	Mix_PlayMusic(music, -1);
	//加载声效
	Mix_Chunk * chunk1 = Mix_LoadWAV(RES_PREFIX "wav/Alarm01.wav");
	Mix_Chunk * chunk2 = Mix_LoadWAV(RES_PREFIX "wav/Alarm02.wav");
	Mix_AllocateChannels(2);

	Uint16 i = 0;
	GIF_Image* gif = GIF_LoadImage(RES_PREFIX "gif/welcome2.gif");
	
	SDL_Rect dstRect = { 0,0,0,0 };

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
			nCurrCounter = SDL_GetPerformanceCounter();
			nElapsedCounter = nCurrCounter - nPrevCounter;
			nPrevCounter = nCurrCounter;

			//前后两帧的耗时(ms)
			elapsed = (nElapsedCounter * 1000.0f) / nFrequency;

			//清除背景
			SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
			SDL_RenderClear(pRenderer);

			//TODO:渲染图像

			/*{
				SDL_Surface* bmp = SDL_LoadBMP("test.bmp");
				SDL_Texture* text = SDL_CreateTextureFromSurface(pRenderer, bmp);
				dstRect.x = 0, dstRect.y = 0, dstRect.w = bmp->w, dstRect.h = bmp->h;
				SDL_RenderCopy(pRenderer, text, NULL, &dstRect);
				SDL_FreeSurface(bmp);
				SDL_DestroyTexture(text);
			}*/
			{
				SDL_Surface* png = IMG_Load(RES_PREFIX "img/test.png");
				SDL_Texture* text = SDL_CreateTextureFromSurface(pRenderer, png);
				dstRect.x = 0, dstRect.y = 0, dstRect.w = png->w, dstRect.h = png->h;
				SDL_RenderCopy(pRenderer, text, NULL, &dstRect);
				SDL_FreeSurface(png);
				SDL_DestroyTexture(text);
			}

			{
				SDL_Color color = { 255, 255, 255, 255 };
				TTF_Font* font = TTF_OpenFont(RES_PREFIX "font/simsun.ttf", 18);
				SDL_Surface* surf = TTF_RenderText_Blended(font, "Are you OK ?", color);
				SDL_Texture* ftex = SDL_CreateTextureFromSurface(pRenderer, surf);
				dstRect.x = 0, dstRect.y = 300, dstRect.w = surf->w, dstRect.h = surf->h;
				SDL_RenderCopy(pRenderer, ftex, NULL, &dstRect);
				SDL_FreeSurface(surf);
				SDL_DestroyTexture(ftex);
			}
			if (i == 0 || (i > 0 && (SDL_GetTicks() - tickStart) >= gif->frames[i]->delay))
			{
				SDL_Texture* texture = SDL_CreateTextureFromSurface(pRenderer, gif->frames[i]->surface);
				dstRect.x = 200, dstRect.y = 0, dstRect.w = gif->frames[i]->surface->w, dstRect.h = gif->frames[i]->surface->h;
				SDL_RenderCopy(pRenderer, texture, NULL, &dstRect);
				SDL_RenderPresent(pRenderer);
				SDL_DestroyTexture(texture);
				tickStart = SDL_GetTicks();
				//SDL_Delay(gif->frames[i]->delay);
				i = (i + 1) % gif->num_frames;
			}
			else
			{
				SDL_Texture* texture = SDL_CreateTextureFromSurface(pRenderer, gif->frames[i]->surface);
				dstRect.x = 200, dstRect.y = 0, dstRect.w = gif->frames[i]->surface->w, dstRect.h = gif->frames[i]->surface->h;
				SDL_RenderCopy(pRenderer, texture, NULL, &dstRect);
				SDL_RenderPresent(pRenderer);
				SDL_DestroyTexture(texture);
			}

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
