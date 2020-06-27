#ifndef UTIL_H
#define UTIL_H

#include <SDL.h>
#include <SDL_ttf.h>


/* Debug print */
#ifdef DEBUG
# define DEBUG_PRINT(...) do { fprintf(stderr, __VA_ARGS__); } while (0)
#else
# define DEBUG_PRINT(...) do { } while (0)
#endif

/* Error print */
#define ERROR_PRINT(...) //do { fprintf(stderr, __VA_ARGS__); } while (0)


/* Some helper macros */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#define MALLOC_ARRAY(type, n) ((type *)malloc(sizeof(type) * n))
#define MALLOC(type) MALLOC_ARRAY(type, 1)
#define CALLOC_ARRAY(type, n) ((type *)calloc(n, sizeof(type)))
#define CALLOC(type) CALLOC_ARRAY(type, 1)
#define STR_EQ(s1, s2) (SDL_strcasecmp(s1, s2) == 0)
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

/* Default text color */
#define DEFAULT_TEXT_COLOR {0, 0, 0, 255}
/* Default foreground color for progress bar */
#define DEFAULT_PB_FG_COLOR {0, 255, 255, 255}
/* Default background color for progress bar */
#define DEFAULT_PB_BG_COLOR {255, 255, 0, 255}

/* Macros to destroy SDL related stuff */
#define DESTROY_IT(it, dtor) \
        if (it) { \
                dtor(it); \
                it = NULL; \
        }

#define DESTROY_SURFACE(it) \
        DESTROY_IT(it, SDL_FreeSurface)

#define DESTROY_CURSOR(it) \
        DESTROY_IT(it, SDL_FreeCursor)

#define DESTROY_TEXTURE(it) \
        DESTROY_IT(it, SDL_DestroyTexture)

#define DESTROY_RENDERER(it) \
        DESTROY_IT(it, SDL_DestroyRenderer)

#define DESTROY_WINDOW(it) \
        DESTROY_IT(it, SDL_DestroyWindow)

#define DESTROY_MUSIC(it) \
        DESTROY_IT(it, Mix_FreeMusic)

#define DESTROY_CHUNK(it) \
        DESTROY_IT(it, Mix_FreeChunk)

#define DESTROY_FONT(it) \
        DESTROY_IT(it, TTF_CloseFont)

/*
 * Random helper
 */

void rand_init(void);

/* min and max is included */
int rand_range(int min, int max);

/*
 * Get file extension of given file name.
 */
const char *get_file_ext(const char *file);

/*
 * Get a random map file.
 */
const char *random_map_file(void);

/*
 * Create a SDL surface from a image file.
 *
 * Support BMP, PNG, JPG format.
 */
SDL_Surface *load_surface(const char *fname);

/*
 * Create a SDL texture from a image file.
 *
 * Support BMP, PNG, JPG format.
 */
SDL_Texture *load_texture(SDL_Renderer *renderer, const char *fname);

/*
 * Draw text on renderer at position (x, y) with
 * given font and color.
 */
void render_text(SDL_Renderer *renderer, TTF_Font *font,
                 const char *text, SDL_Color color, int x, int y);

/*
 * Draw a horizental progress bar on renderer at position (x, y)
 * with width w, and height h.
 * percent range in 0.0 to 1.0.
 */
void render_h_pb(SDL_Renderer *renderer, int x, int y, int w, int h,
                 float percent, SDL_Color fg_color, SDL_Color bg_color);

#endif /* UTIL_H */
