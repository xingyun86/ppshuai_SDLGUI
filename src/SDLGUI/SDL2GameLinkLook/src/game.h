#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "window.h"
#include "tile.h"
#include "map.h"

struct Game {
        bool running;
        struct Window *window;
        struct Map *map;
        struct Tile *tile_bg;
        TTF_Font *font;
        SDL_Texture *tex_outline;
        struct Point pt1, pt2; /* The two selected point */
};

extern bool game_init(struct Game *game);

extern void game_mainloop(struct Game *game);

extern void game_destroy(struct Game *game);

#endif /* GAME_H */
