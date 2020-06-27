#ifndef MIX_CACHE_H
#define MIX_CACHE_H

#include <stdbool.h>
#include <SDL_mixer.h>

enum MixType {
        MIX_TYPE_START,
        MIX_TYPE_MIN = MIX_TYPE_START,
        MIX_TYPE_BACKGROUND,
        MIX_TYPE_SELECT,
        MIX_TYPE_BOOM,
        MIX_TYPE_ELEC,
        MIX_TYPE_MAX = MIX_TYPE_ELEC,
        MIX_TYPE_NUMBER
};

bool mix_cache_init(void);

Mix_Chunk *mix_cache_get_chunk(enum MixType type);

/* Call me even mix_cache_init() failed */
void mix_cache_destroy(void);

#endif /* MIX_CACHE_H */
