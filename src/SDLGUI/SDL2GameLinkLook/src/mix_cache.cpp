#include <stdio.h>

#include "assets.h"
#include "util.h"
#include "mix_cache.h"

#define INIT_MIX(asset, type) \
        chunk = Mix_LoadWAV(asset); \
        if (!chunk) { \
                ERROR_PRINT("Failed to load %s, SDL Error is %s\n", asset, SDL_GetError()); \
                return false; \
        } \
        cache[type] = chunk;

static Mix_Chunk *cache[MIX_TYPE_NUMBER];

bool mix_cache_init(void)
{
        Mix_Chunk *chunk = NULL;

        INIT_MIX(JOIN_AUDIO("bgm.wav"), MIX_TYPE_BACKGROUND);
        INIT_MIX(JOIN_AUDIO("select.wav"), MIX_TYPE_SELECT);
        INIT_MIX(JOIN_AUDIO("boom.wav"), MIX_TYPE_BOOM);
        INIT_MIX(JOIN_AUDIO("elec.wav"), MIX_TYPE_ELEC);

        return true;
}

Mix_Chunk *mix_cache_get_chunk(enum MixType type)
{
        return cache[type];
}

void mix_cache_destroy(void)
{
        for (int i = MIX_TYPE_MIN; i <= MIX_TYPE_MAX; i++) {
                DESTROY_CHUNK(cache[i]);
        }
}
