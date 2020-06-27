#include <stdio.h>

#include "tex_cache.h"
#include "mix_cache.h"
#include "assets.h"
#include "path.h"
#include "game.h"

#define handle_init_error(msg) \
        fprintf(stderr, "%s, SDL_Error: %s\n", msg, SDL_GetError()); \
        game_destroy(game); \
        return false

/* Some handy macros to access field from `game'  */
#define _running        (game->running)
#define _window         (game->window)
#define _renderer       (game->window->renderer)
#define _font           (game->font)
#define _tile_bg        (game->tile_bg)
#define _tex_outline    (game->tex_outline)
#define _map            (game->map)
#define _pt1            (game->pt1)
#define _pt2            (game->pt2)

static bool game_start(struct Game *game)
{
        map_destroy(_map);
        
        if (!map_init(_map)) {
                handle_init_error("Map could not initialized");
        }

        EMPTY_POINT(_pt1);
        EMPTY_POINT(_pt2);
        _running = true;
        
        return true;
}

bool game_init(struct Game *game)
{
        rand_init();

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
                handle_init_error("SDL could not initialized");
        }
        
        int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
        if (!(IMG_Init(img_flags) & img_flags)) {
                handle_init_error("SDL_image could not initialized");
        }

        if (TTF_Init() == -1) {
                handle_init_error("SDL_font could not initialized");
        }

        if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) < 0) {
                handle_init_error("SDL_mixer could not initialized");
        }

        _window = MALLOC(struct Window);
        if (!_window) {
                handle_init_error("Memory could not allocated for window");
        }

        if (!window_init(_window)) {
                handle_init_error("Window could not initialized");
        }

        _font = TTF_OpenFont(JOIN_FONT("SimSun.ttf"), 20);
        if (!_font) {
                handle_init_error("Font " JOIN_FONT("SimSun.ttf")  "could not open");
        }

        if (!tex_cache_init(_renderer)) {
                handle_init_error("Texture cache could not initialized");
        }

        if (!mix_cache_init()) {
                handle_init_error("Mixer cache could not initialized");
        }

        _tile_bg = tile_new(TEX_TYPE_BACKGROUND, NULL, 0, 0);
        if (!_tile_bg) {
                handle_init_error("Background tile could not initialized");
        }

        _tex_outline = tex_cache_get_texture(TEX_TYPE_OUTLINE);

        _map = map_new(random_map_file());
        if (!_map) {
                handle_init_error("Memory could not allocated for map");
        }

        Mix_PlayChannel(-1, mix_cache_get_chunk(MIX_TYPE_BACKGROUND), -1);

        if (!game_start(game)) {
                handle_init_error("Game could not start");
        }

        return true;
}

void game_destroy(struct Game *game)
{
        DESTROY_FONT(_font);
        
        tex_cache_destroy();
        mix_cache_destroy();
        map_destroy(_map);
        tile_destroy(_tile_bg);
        window_destroy(_window);

        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
}

static const char *get_hint_text(struct Game *game)
{
        static char buf[64];

        snprintf(buf, sizeof(buf), "剩余方块数: %d/%d",
                 _map->tile_cnt_left, _map->tile_cnt_total);

        return buf;
}

static void draw_lighting_line(struct Game *game,
                               const struct Point *pt1, const struct Point *pt2)
{
        int dx, dy;
        enum Direction dir;
        enum TexType tex_type;

        dir = get_line_dir(pt1, pt2);
        switch (dir) {
        case DIR_LEFT:
                dx = -1, dy = 0;
                tex_type = TEX_TYPE_LIGHTING_H;
                break;
        case DIR_RIGHT:
                dx = 1, dy = 0;
                tex_type = TEX_TYPE_LIGHTING_H;
                break;
        case DIR_DOWN:
                dx = 0, dy = 1;
                tex_type = TEX_TYPE_LIGHTING_V;
                break;
        case DIR_UP:
                dx = 0, dy = -1;
                tex_type = TEX_TYPE_LIGHTING_V;
                break;
        default:
                /* Do nothing */
                return;
        }

        Uint32 tick = SDL_GetTicks();
        int x = pt1->x + dx, y = pt1->y + dy;
        while (x != pt2->x || y != pt2->y) {
                tile_set_animation(map_get_tile_xy(_map, x, y), tex_type,
                                   tick, tick + TILE_ANIMATION_TICKS);
                x += dx, y += dy;
        }
}

static void draw_corner(struct Game *game, const struct Point *pt1,
                        const struct Point *pt2, const struct Point *corner)
{
        enum TexType tex_type;
        enum Direction dir;

        dir = get_corner_dir(pt1, pt2, corner);
        switch (dir) {
        case DIR_LT:
                tex_type = TEX_TYPE_LIGHTING_LT;
                break;
        case DIR_LB:
                tex_type = TEX_TYPE_LIGHTING_LB;
                break;
        case DIR_RT:
                tex_type = TEX_TYPE_LIGHTING_RT;
                break;
        case DIR_RB:
                tex_type = TEX_TYPE_LIGHTING_RB;
                break;
        default:
                /* Do nothing */
                return;
        }

        Uint32 tick = SDL_GetTicks();
        tile_set_animation(map_get_tile_pt(_map, corner), tex_type,
                           tick, tick + TILE_ANIMATION_TICKS);
}

static void draw_lighting(struct Game *game, enum PathType path,
                          const struct Point *corner1, const struct Point *corner2)
{
        if (path == PATH_ZERO_CORNER) {
                draw_lighting_line(game, &_pt1, &_pt2);
        } else if (path == PATH_ONE_CORNER) {
                draw_lighting_line(game, &_pt1, corner1);
                draw_corner(game, &_pt1, &_pt2, corner1);
                draw_lighting_line(game, corner1, &_pt2);
        } else if (path == PATH_TWO_CORNER) {
                draw_lighting_line(game, &_pt1, corner1);
                draw_corner(game, &_pt1, corner2, corner1);
                draw_lighting_line(game, corner1, corner2);
                draw_corner(game, corner1, &_pt2, corner2);
                draw_lighting_line(game, corner2, &_pt2);
        }
}

/* Core step to eliminate two tiles */
static void mahjong_it(struct Game *game)
{
        struct Point corner1, corner2;
        enum PathType path = find_path(_map, &_pt1, &_pt2, &corner1, &corner2);
        
        if (path == PATH_NOT_FOUND) {
                _pt1 = _pt2;
                EMPTY_POINT(_pt2);
                return;
        }

        Mix_PlayChannel(-1, mix_cache_get_chunk(MIX_TYPE_BOOM), 0);
        Mix_PlayChannel(-1, mix_cache_get_chunk(MIX_TYPE_ELEC), 0);

        /* Boom pt1 and pt2 */
        Uint32 tick = SDL_GetTicks();
        tile_set_animation(map_get_tile_pt(_map, &_pt1), TEX_TYPE_BOOM,
                           tick, tick + TILE_ANIMATION_TICKS);
        tile_set_animation(map_get_tile_pt(_map, &_pt2), TEX_TYPE_BOOM,
                           tick, tick + TILE_ANIMATION_TICKS);

        draw_lighting(game, path, &corner1, &corner2);
        
        /* Clear pt1 and pt2 */
        EMPTY_POINT(_pt1);
        EMPTY_POINT(_pt2);

        _map->tile_cnt_left -= 2;
}

/* Handler for mouse event */
static void game_mouse_press(struct Game *game, SDL_MouseButtonEvent event)
{
        if (event.button == SDL_BUTTON_LEFT) {  /* left click */
                int x, y;
                x = event.x / TILE_WIDTH;
                y = event.y / TILE_HEIGHT;

                if (!map_is_barrier_xy(_map, x, y)) {
                        return;
                }

                /* Play select sound effect */
                Mix_PlayChannel(-1, mix_cache_get_chunk(MIX_TYPE_SELECT), 0);

                if (POINT_IS_NULL(_pt1)) {
                        POINT_SET(_pt1, x, y);
                        return;
                }
                
                POINT_SET(_pt2, x, y);

                if (POINT_EQ(_pt1, _pt2) ||
                    !map_eq_pt(_map, &_pt1, &_pt2)) {
                        _pt1 = _pt2;
                        EMPTY_POINT(_pt2);
                        return;
                }

                mahjong_it(game);
        }
}

/* Handler for SDL event */
static void game_event_handler(struct Game *game)
{
        SDL_Event e;
        
        while (SDL_PollEvent(&e)) {
                switch (e.type) {
                case SDL_QUIT:
                        _running = false; /* Quit */
                        break;
                case SDL_MOUSEBUTTONDOWN:
                        game_mouse_press(game, e.button);
                        break;
                }
        }
}

/* Render the game */
static void game_render(struct Game *game)
{
        SDL_RenderClear(_renderer);

        /* Render the background texture */
        tile_render(_renderer, _tile_bg);

        /* Render the map */
        MAP_FOREACH_BEGIN {
                tile_render(_renderer, map_get_tile_xy(_map, col, row));
        } MAP_FOREACH_END;

        /* Render outline for tile at pt1 */ 
        if (!POINT_IS_NULL(_pt1)) {
                SDL_Rect rect = TILE_INIT_RECT(_pt1.x, _pt1.y);
                SDL_RenderCopy(_renderer, _tex_outline, NULL, &rect);
        }

        /* Render the hint text */
        SDL_Color text_color = DEFAULT_TEXT_COLOR;
        render_text(_renderer, _font, get_hint_text(game),
                    text_color, HINT_TEXT_X, HINT_TEXT_Y);

        SDL_Color fg_color = DEFAULT_PB_FG_COLOR;
        SDL_Color bg_color = DEFAULT_PB_BG_COLOR;
        render_h_pb(_renderer, H_PB_X, H_PB_Y, H_PB_WIDTH, H_PB_HEIGHT, 0.3, fg_color, bg_color);
        
        SDL_RenderPresent(_renderer);
}

void game_mainloop(struct Game *game)
{
        while (_running) {
                game_event_handler(game);
                game_render(game);
        }
}
