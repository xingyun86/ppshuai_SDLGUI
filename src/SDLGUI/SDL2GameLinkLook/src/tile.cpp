#include "util.h"
#include "tile.h"

struct Tile *tile_new(enum TexType tex_type, SDL_Rect *rect,
                      Uint32 tick_start, Uint32 tick_end)
{
        struct Tile *tile = NULL;

        tile = MALLOC(struct Tile);
        if (!tile) {
                return NULL;
        }
        
        tile->rect = rect;
        tile_set_animation(tile, tex_type, tick_start, tick_end);

        return tile;
}

void tile_render(SDL_Renderer *renderer, struct Tile *tile)
{
        if (!tile || tile_is_none(tile)) {
                return;
        }

        if (tile->tick_end != 0 && tile->tick_end < tile->tick_start) {
                return;
        }

        SDL_RenderCopy(renderer, tile->texture, NULL, tile->rect);

        tile->tick_start = SDL_GetTicks();
}

void tile_set_animation(struct Tile *tile, enum TexType tex_type,
                        Uint32 tick_start, Uint32 tick_end)
{
        if (!tile) {
                return;
        }
        
        tile->tex_type = tex_type;
        tile->texture = tex_cache_get_texture(tex_type);
        tile->tick_start = tick_start;
        tile->tick_end = tick_end;
}

void tile_destroy(struct Tile *tile)
{
        if (tile) {
                free(tile->rect);
                free(tile);
        }
}

bool tile_eq(const struct Tile *tile1, const struct Tile *tile2)
{
        if (!tile1 || !tile2) {
                return false;
        }
        
        return (tile1 == tile2 ||
                tile1->tex_type == tile2->tex_type);
}

bool tile_is_barrier(const struct Tile *tile)
{
        if (!tile) {
                return false;
        }
        
        enum TexType tex_type = tile->tex_type;
        return (tex_type >= TEX_TYPE_TILE_MIN &&
                tex_type <= TEX_TYPE_TILE_MAX);
}

bool tile_is_none(const struct Tile *tile)
{
        if (!tile) {
                return false;
        }
        
        return tile->tex_type == TEX_TYPE_NONE;
}
