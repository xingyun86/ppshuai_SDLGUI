#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL_image.h>

#include "assets.h"
#include "util.h"


void rand_init(void)
{
        srand(time(NULL));
}

int rand_range(int min, int max)
{
        int low, high;
        
        if (min < max) {
                low = min;
                high = max + 1;
        } else if (min > max) {
                low = max + 1;
                high = min;
        } else {
                return min;
        }

        return (rand() % (high - low)) + low;
}

const char *get_file_ext(const char *file)
{
        if (!file) {
                return NULL;
        }
        
        return SDL_strrchr(file, '.') + 1;
}

const char *random_map_file(void)
{
        static const char *map_files[] = {
                JOIN_MAP("map1.txt"),
                JOIN_MAP("map2.txt"),
                JOIN_MAP("map3.txt"),
                JOIN_MAP("map4.txt"),
        };

        return map_files[rand_range(0, ARRAY_SIZE(map_files) - 1)];
}


SDL_Surface *load_surface(const char *fname)
{
        const char *ext = NULL;
        SDL_Surface *surface = NULL;

        if (!fname) {
                return NULL;
        }

        ext = get_file_ext(fname);
        if (STR_EQ(ext, "bmp")) {
                surface = SDL_LoadBMP(fname);
        } else if (STR_EQ(ext, "png") ||
                   STR_EQ(ext, "jpg")) {
                surface = IMG_Load(fname);
        } else if (STR_EQ(ext, "cur")) {
                SDL_RWops *rwop = SDL_RWFromFile(fname, "rb");
                if (!rwop) {
                        goto RETURN;
                }

                surface = IMG_LoadCUR_RW(rwop);

                /* No longer required */
                SDL_FreeRW(rwop);
        }

RETURN:
        if (!surface) {
                ERROR_PRINT("Unable to load image %s, SDL_Error: %s\n", fname, SDL_GetError());
        }
        
        return surface;
}

SDL_Texture *load_texture(SDL_Renderer *renderer, const char *fname)
{
        SDL_Surface *surface = NULL;
        SDL_Texture *texture = NULL;

        if (!renderer || !fname) {
                return NULL;
        }
        
        surface = load_surface(fname);
        if (!surface) {
                return NULL;
        }

        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
                ERROR_PRINT("Unable to create texture from %s, SDL_Error: %s\n", fname, SDL_GetError());
        }

        DESTROY_SURFACE(surface);

        return texture;
}

void render_text(SDL_Renderer *renderer, TTF_Font *font,
                 const char *text, SDL_Color color, int x, int y)
{
        SDL_Surface *surface;
        SDL_Texture *texture;
        SDL_Rect rect = { 0,0,0,0 };
        surface = TTF_RenderUTF8_Solid(font, text, color);
        if (!surface) {
                return;
        }

        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
                goto FREE;
        }

        rect.x = x, rect.y = y, rect.w = surface->w, rect.h = surface->h;

        SDL_RenderCopy(renderer, texture, NULL, &rect);

FREE:
        DESTROY_SURFACE(surface);
        DESTROY_TEXTURE(texture);
}

void render_h_pb(SDL_Renderer *renderer, int x, int y, int w, int h,
                 float percent, SDL_Color fg_color, SDL_Color bg_color)
{
        percent = percent > 1.f ? 1.f : percent < 0.f ? 0.f : percent;
        
        SDL_Color old;
        SDL_GetRenderDrawColor(renderer, &old.r, &old.g, &old.g, &old.a);

        SDL_Rect bgrect = { x, y, w, h };
        SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
        SDL_RenderFillRect(renderer, &bgrect);

        SDL_SetRenderDrawColor(renderer, fg_color.r, fg_color.g, fg_color.b, fg_color.a);
        int pw = (int)((float)w * percent);
        int px = x + (w - pw);
        SDL_Rect fgrect = { px, y, pw, h };
        SDL_RenderFillRect(renderer, &fgrect);

        SDL_SetRenderDrawColor(renderer, old.r, old.g, old.b, old.a);
}
