#ifndef TILE_H
#define TILE_H

#include <SDL.h>

#include "tex_cache.h"

#define TILE_WIDTH 35
#define TILE_HEIGHT 39
#define TILE_INIT_RECT(x, y) {x * TILE_WIDTH, y * TILE_HEIGHT, \
                              TILE_WIDTH, TILE_HEIGHT}
#define TILE_SET_RECT(rect, _x, _y) \
        do { \
                rect->x = _x * TILE_WIDTH;      \
                rect->y = _y * TILE_HEIGHT;     \
                rect->w = TILE_WIDTH;           \
                rect->h = TILE_HEIGHT;          \
        } while (0)

#define TILE_ANIMATION_TICKS 500

struct Tile {
        enum TexType tex_type;
        SDL_Texture *texture;
        SDL_Rect *rect;
        /* For animation */
        Uint32 tick_start;
        Uint32 tick_end;  /* 0 for infinite */
};

struct Tile *tile_new(enum TexType tex_type, SDL_Rect *rect,
                      Uint32 tick_start, Uint32 tick_end);

void tile_destroy(struct Tile *tile);

void tile_render(SDL_Renderer *renderer, struct Tile *tile);

void tile_set_animation(struct Tile *tile, enum TexType tex_type,
                        Uint32 tick_start, Uint32 tick_end);

bool tile_is_barrier(const struct Tile *tile);

bool tile_is_none(const struct Tile *tile);

bool tile_eq(const struct Tile *tile1, const struct Tile *tile2);

#endif /* TILE_H */
