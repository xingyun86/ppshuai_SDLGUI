#include "path.h"

static bool get_line(const struct Map *map,
                     const struct Point *pt1, const struct Point *pt2)
{
        bool valid;

        if (pt1->x == pt2->x) {  /* Vertical */
                valid = true;
                
                int min_y = MIN(pt1->y, pt2->y) + 1;
                int max_y = MAX(pt1->y, pt2->y) - 1;
                for (int y = min_y; y <= max_y; y++) {
                        if (map_is_barrier_xy(map, pt1->x, y)) {
                                valid = false;
                                break;
                        }
                }

                if (valid) {
                        return true;
                }
        }

        /* Eliminate the duplicate code?? */
        if (pt1->y == pt2->y) {  /* Horizental */
                valid = true;
                
                int min_x = MIN(pt1->x, pt2->x) + 1;
                int max_x = MAX(pt1->x, pt2->x) - 1;
                for (int x = min_x; x <= max_x; x++) {
                        if (map_is_barrier_xy(map, x, pt1->y)) {
                                valid = false;
                                break;
                        }
                }

                if (valid) {
                        return true;
                }
        }

        return false;
}

static bool get_line_with_corner(const struct Map *map,
                                 const struct Point *pt1, const struct Point *pt2,
                                 const struct Point *corner)
{
        if (map_is_barrier_pt(map, corner)) {
                return false;
        }

        return (get_line(map, pt1, corner) &&
                get_line(map, corner, pt2));
}


static bool zero_corner(const struct Map *map,
                        const struct Point *pt1, const struct Point *pt2)
{
        return get_line(map, pt1, pt2);
}


static bool one_corner(const struct Map *map,
                       const struct Point *pt1, const struct Point *pt2,
                       struct Point *corner)
{
        /*
         * corner ----------- pt2
         *  |
         *  |
         *  |
         * pt1
         */
        POINT_SET(*corner, pt1->x, pt2->y);
        if (get_line_with_corner(map, pt1, pt2, corner)) {
                return true;
        }

        /*
         * pt1 ------------ corner
         *                    |
         *                    |
         *                    |
         *                    pt2
         */
        POINT_SET(*corner, pt2->x, pt1->y);
        if (get_line_with_corner(map, pt1, pt2, corner)) {
                return true;
        }

        return false;
}

static bool two_corner(const struct Map *map,
                       const struct Point *pt1, const struct Point *pt2,
                       struct Point *corner1, struct Point *corner2)
{
        /*
         * corner1 -------------- corner2
         *  |                        |
         *  |                        |
         *  |                        |
         *  pt1                     pt2
         */
        for (int x = 0; x < MAP_COLS; x++) {
                if (x == pt1->x || x == pt2->x) {
                        continue;
                }

                POINT_SET(*corner1, x, pt1->y);
                POINT_SET(*corner2, x, pt2->y);
                if (get_line_with_corner(map, pt1, corner2, corner1) &&
                    get_line_with_corner(map, corner1, pt2, corner2)) {
                        return true;
                }
        }

        /*
         * pt1 -------- corner1
         *                |
         *                |
         *                |
         * pt2 -------- corner2
         */
        for (int y = 0; y < MAP_ROWS; y++) {
                if (y == pt1->y || y == pt2->y) {
                        continue;
                }

                POINT_SET(*corner1, pt1->x, y);
                POINT_SET(*corner2, pt2->x, y);
                if (get_line_with_corner(map, pt1, corner2, corner1) &&
                    get_line_with_corner(map, corner1, pt2, corner2)) {
                        return true;
                }
        }

        return false;
}

enum PathType find_path(const struct Map *map,
                        const struct Point *pt1, const struct Point *pt2,
                        struct Point *corner1, struct Point *corner2)
{
        if (!map || !pt1 || !pt2 || !corner1 || !corner2) {
                return PATH_NOT_FOUND;
        }
        
        if (zero_corner(map, pt1, pt2)) {
                return PATH_ZERO_CORNER;
        }

        if (one_corner(map, pt1, pt2, corner1)) {
                return PATH_ONE_CORNER;
        }

        if (two_corner(map, pt1, pt2, corner1, corner2)) {
                return PATH_TWO_CORNER;
        }

        return PATH_NOT_FOUND;
}
