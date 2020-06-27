#ifndef WINDOW_H
#define WINDOW_H

#include <stdbool.h>
#include <SDL.h>

#include "tile.h"
#include "map.h"

/* Window metrics */
#define WINDOW_X       (SDL_WINDOWPOS_UNDEFINED)
#define WINDOW_Y       (SDL_WINDOWPOS_UNDEFINED)
#define MAP_WIDTH      (TILE_WIDTH * MAP_COLS)
#define MAP_HEIGHT     (TILE_HEIGHT * MAP_ROWS)
#define BOTTOM_PADDING 100
#define HINT_TEXT_X    MAP_WIDTH - 200
#define HINT_TEXT_Y    MAP_HEIGHT + BOTTOM_PADDING / 2 - 10
#define H_PB_X         20
#define H_PB_Y         HINT_TEXT_Y
#define H_PB_WIDTH     430
#define H_PB_HEIGHT    20
#define WINDOW_WIDTH   MAP_WIDTH
#define WINDOW_HEIGHT  MAP_HEIGHT + BOTTOM_PADDING

struct Window {
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Cursor *cursor;
};

bool window_init(struct Window *window);

void window_destroy(struct Window *window);

#endif /* WINDOW_H */
