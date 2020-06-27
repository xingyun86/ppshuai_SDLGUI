#include <stdio.h>
#include <inttypes.h>

#include "util.h"
#include "tex_cache.h"
#include "map.h"

static bool generate_pair_tiles(struct Map *map, struct Point **non_zero_points)
{
        int tile_cnt_total = map->tile_cnt_total;
        struct Point *point;
        int point_index;
        enum TexType tex_type;

        while (tile_cnt_total > 0) {
                tex_type = (enum TexType)rand_range(TEX_TYPE_TILE_MIN, TEX_TYPE_TILE_MAX);

                /* Generate a pair with the same image index */
                for (int i = 0; i < 2; i++) {
                        point_index = rand_range(0, --tile_cnt_total);
                        point = non_zero_points[point_index];

                        SDL_Rect *rect = MALLOC(SDL_Rect);
                        TILE_SET_RECT(rect, point->x, point->y);
                        if (!(map->tiles[point->y][point->x] = tile_new(tex_type, rect, 0, 0))) {
                                ERROR_PRINT("(%d, %d) - %d new tile failed", point->x, point->y, tex_type);
                                return false;
                        }

                        /* `Remove' the point from non_zero_points on index */
                        *non_zero_points[point_index] = *non_zero_points[tile_cnt_total];
                }
        }

        return true;
}

static bool init_tiles(struct Map *map, signed char masks[MAP_ROWS][MAP_COLS])
{
        struct Point **non_zero_points = NULL;
        
        non_zero_points = MALLOC_ARRAY(struct Point*, MAP_ROWS * MAP_COLS);
        if (!non_zero_points) {
                return false;
        }

        MAP_FOREACH_BEGIN {
                if (masks[row][col]) {
                        struct Point *point = MALLOC(struct Point);
                        POINT_SET(*point, col, row);
                        non_zero_points[map->tile_cnt_total++] = point;
                } else {
                        SDL_Rect *rect = MALLOC(SDL_Rect);
                        TILE_SET_RECT(rect, col, row);
                        map->tiles[row][col] = tile_new(TEX_TYPE_NONE, rect, 0, 0);
                }
        } MAP_FOREACH_END;

        bool result = generate_pair_tiles(map, non_zero_points);

        if (non_zero_points) {
                for (int i = 0; i < map->tile_cnt_total; i++) {
                        free(non_zero_points[i]);
                }
                free(non_zero_points);
        }
        
        return result;
}

struct Map *map_new(const char *map_file)
{
        struct Map *map = MALLOC(struct Map);
        if (!map) {
                return NULL;
        }

        map->initialized = false;
        map->map_file = map_file;
        map->tile_cnt_total = 0;
        map->tile_cnt_left = 0;
        SDL_memset(map->tiles, 0, sizeof(map->tiles));

        return map;
}

bool map_init(struct Map *map)
{
        FILE *fp = NULL;
        signed char masks[MAP_ROWS][MAP_COLS];
        bool success = false;

        if (!map) {
                return false;
        }
        
        fp = fopen(map->map_file, "r");
        if (!fp) {
                return false;
        }

        MAP_FOREACH_BEGIN {
                if (fscanf(fp, "%" SCNi8, &masks[row][col]) != 1) {
                        goto EXIT;
                }
        } MAP_FOREACH_END;

        if (!init_tiles(map, masks)) {
                goto EXIT;
        }

        success = true;
        
EXIT:
        map->tile_cnt_left = map->tile_cnt_total;
        map->initialized = true;
        fclose(fp);
        return success;
}

void map_destroy(struct Map *map)
{
        if (!map || !map->initialized) {
                return;
        }
        
        MAP_FOREACH_BEGIN {
                tile_destroy(map_get_tile_xy(map, col, row));
        } MAP_FOREACH_END;

        free(map);
}

inline static bool point_in_map(int x, int y)
{
        return (x >= 0 && x < MAP_COLS &&
                y >= 0 && y < MAP_ROWS);
}

struct Tile *map_get_tile_xy(const struct Map *map, int x, int y)
{
        if (!map || !point_in_map(x, y)) {
                return NULL;
        }

        return map->tiles[y][x];
}

struct Tile *map_get_tile_pt(const struct Map *map, const struct Point *pt)
{
        if (!map || !pt || !point_in_map(pt->x, pt->y)) {
                return NULL;
        }

        return map->tiles[pt->y][pt->x];
}

bool map_is_none_xy(const struct Map *map, int x, int y)
{
        if (!map || !point_in_map(x, y)) {
                return false;
        }

        return tile_is_none(map_get_tile_xy(map, x, y));
}

bool map_is_none_pt(const struct Map *map, const struct Point *pt)
{
        if (!map || !pt || !point_in_map(pt->x, pt->y)) {
                return false;
        }

        return tile_is_none(map_get_tile_pt(map, pt));
}

bool map_is_barrier_xy(const struct Map *map, int x, int y)
{
        if (!map || !point_in_map(x, y)) {
                return false;
        }

        return tile_is_barrier(map_get_tile_xy(map, x, y));
}

bool map_is_barrier_pt(const struct Map *map, const struct Point *pt)
{
        if (!map || !point_in_map(pt->x, pt->y)) {
                return false;
        }

        return tile_is_barrier(map_get_tile_pt(map, pt));
}

bool map_eq_pt(const struct Map *map, const struct Point *pt1, const struct Point *pt2)
{
        if (!map || !pt1 || !pt2) {
                return false;
        }

        return tile_eq(map_get_tile_pt(map, pt1), map_get_tile_pt(map, pt2));
}
