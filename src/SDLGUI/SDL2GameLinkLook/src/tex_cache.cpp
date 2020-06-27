#include <stdio.h>
#include <inttypes.h>
#include "assets.h"
#include "tex_cache.h"

#define SPECIAL_TEX_TABLE \
        X(JOIN_IMAGE("bg.jpg"), TEX_TYPE_BACKGROUND)    \
        X(JOIN_IMAGE("outline.png"), TEX_TYPE_OUTLINE)  \
        X(JOIN_IMAGE("boom.png"), TEX_TYPE_BOOM)        \
        X(JOIN_IMAGE("h.png"), TEX_TYPE_LIGHTING_H)     \
        X(JOIN_IMAGE("v.png"), TEX_TYPE_LIGHTING_V)     \
        X(JOIN_IMAGE("lt.png"), TEX_TYPE_LIGHTING_LT)   \
        X(JOIN_IMAGE("rt.png"), TEX_TYPE_LIGHTING_RT)   \
        X(JOIN_IMAGE("lb.png"), TEX_TYPE_LIGHTING_LB)   \
        X(JOIN_IMAGE("rb.png"), TEX_TYPE_LIGHTING_RB)

/* The texture cache */
static SDL_Texture *cache[TEX_TYPE_NUMBER];

static bool init_tex_tiles(SDL_Renderer *renderer)
{
        SDL_Texture *texture = NULL;
        char *buf = NULL;
        int max_len = 0;
        const char* tex_item_tpl = IMAGE_DIR "item%" SCNi32 ".png";

        max_len = snprintf(buf, 0, tex_item_tpl, TEX_TYPE_TILE_MAX) + 1;
        buf = (char *)malloc(max_len);

        for (int i = TEX_TYPE_TILE_MIN; i <= TEX_TYPE_TILE_MAX; i++) {
                snprintf(buf, max_len, tex_item_tpl, i);
                texture = load_texture(renderer, buf);
                if (!texture) {
                        free(buf);
                        return false;
                }
                cache[i] = texture;
        }

        free(buf);
        return true;
}

bool tex_cache_init(SDL_Renderer *renderer)
{
        SDL_Texture *texture = NULL;

        cache[TEX_TYPE_NONE] = NULL;
        
#define X(asset, type) \
        texture = load_texture(renderer, asset); \
        if (!texture) { \
                ERROR_PRINT("Failed to load %s, SDL_Error: %s\n", asset, SDL_GetError()); \
                return false; \
        } \
        cache[type] = texture;

        SPECIAL_TEX_TABLE
#undef X

        if (!init_tex_tiles(renderer)) {
                return false;
        }

        return true;
}

SDL_Texture *tex_cache_get_texture(enum TexType type)
{
        return cache[type];
}

void tex_cache_destroy(void)
{
        for (int i = TEX_TYPE_NONE; i < TEX_TYPE_NUMBER; i++) {
                DESTROY_TEXTURE(cache[i]);
        }
}
