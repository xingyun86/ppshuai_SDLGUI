#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

#include "point.h"
#include "tile.h"

#define MAP_ROWS 11
#define MAP_COLS 19

#define MAP_FOREACH_BEGIN \
        for (int row = 0; row < MAP_ROWS; row++) { \
        for (int col = 0; col < MAP_COLS; col++) {
#define MAP_FOREACH_END }}

struct Map {
        bool initialized;
        const char *map_file;
        
        struct Tile *tiles[MAP_ROWS][MAP_COLS];
        int tile_cnt_total;  /* total count of non TEX_TYPE_NONE tiles */
        int tile_cnt_left;  /* left count of non TEX_TYPE_NONE tiles */
};

struct Map *map_new(const char *map_file);

bool map_init(struct Map *map);

void map_destroy(struct Map *map);

struct Tile *map_get_tile_xy(const struct Map *map, int x, int y);
struct Tile *map_get_tile_pt(const struct Map *map, const struct Point *pt);

bool map_is_none_xy(const struct Map *map, int x, int y);
bool map_is_none_pt(const struct Map *map, const struct Point *pt);

bool map_is_barrier_xy(const struct Map *map, int x, int y);
bool map_is_barrier_pt(const struct Map *map, const struct Point *pt);

bool map_eq_pt(const struct Map *map, const struct Point *pt1, const struct Point *pt2);

#endif /* MAP_H */
