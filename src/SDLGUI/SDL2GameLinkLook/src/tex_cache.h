#ifndef TEX_CACHE_H
#define TEX_CACHE_H

#include <stdbool.h>
#include <SDL.h>

#include "util.h"

enum TexType {
        TEX_TYPE_NONE,
        /* TODO: `Optimize` with sequence iterator macro */
        TEX_TYPE_TILE_1,  /* Remember to update TEX_TYPE_TILE_MIN */
        TEX_TYPE_TILE_MIN = TEX_TYPE_TILE_1,
        TEX_TYPE_TILE_2,
        TEX_TYPE_TILE_3,
        TEX_TYPE_TILE_4,
        TEX_TYPE_TILE_5,
        TEX_TYPE_TILE_6,
        TEX_TYPE_TILE_7,
        TEX_TYPE_TILE_8,
        TEX_TYPE_TILE_9,
        TEX_TYPE_TILE_10,
        TEX_TYPE_TILE_11,
        TEX_TYPE_TILE_12,
        TEX_TYPE_TILE_13,
        TEX_TYPE_TILE_14,
        TEX_TYPE_TILE_15,
        TEX_TYPE_TILE_16,
        TEX_TYPE_TILE_17,
        TEX_TYPE_TILE_18,
        TEX_TYPE_TILE_19,
        TEX_TYPE_TILE_20,
        TEX_TYPE_TILE_21,
        TEX_TYPE_TILE_22,
        TEX_TYPE_TILE_23,
        TEX_TYPE_TILE_24,
        TEX_TYPE_TILE_25,
        TEX_TYPE_TILE_26,
        TEX_TYPE_TILE_27,
        TEX_TYPE_TILE_28,
        TEX_TYPE_TILE_29,
        TEX_TYPE_TILE_30,
        TEX_TYPE_TILE_31,
        TEX_TYPE_TILE_32,
        TEX_TYPE_TILE_33,
        TEX_TYPE_TILE_34,
        TEX_TYPE_TILE_35,
        TEX_TYPE_TILE_36,
        TEX_TYPE_TILE_37,
        TEX_TYPE_TILE_38,
        TEX_TYPE_TILE_39,
        TEX_TYPE_TILE_40,  /* Remember to update TEX_TYPE_TILE_MAX */
        TEX_TYPE_TILE_MAX = TEX_TYPE_TILE_40,
        TEX_TYPE_BACKGROUND,
        TEX_TYPE_BOOM,
        TEX_TYPE_OUTLINE,
        TEX_TYPE_LIGHTING_H,
        TEX_TYPE_LIGHTING_V,
        TEX_TYPE_LIGHTING_LT,
        TEX_TYPE_LIGHTING_RT,
        TEX_TYPE_LIGHTING_LB,
        TEX_TYPE_LIGHTING_RB,
        TEX_TYPE_NUMBER
};

bool tex_cache_init(SDL_Renderer *renderer);

SDL_Texture *tex_cache_get_texture(enum TexType type);

/* Call me even tex_cache_init() failed */
void tex_cache_destroy(void);

#endif /* TEX_CACHE_H */
