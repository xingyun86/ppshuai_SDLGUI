#include "point.h"
#include "util.h"

enum Direction get_line_dir(const struct Point *pt1, const struct Point *pt2)
{
        if (!pt1 || !pt2) {
                return DIR_UNKNOWN;
        }

        if (pt1->x < pt2->x) {
                /* pt1 --- pt2 */
                return DIR_RIGHT;
        } else if (pt1->x > pt2->x) {
                /* pt2 --- pt1 */
                return DIR_LEFT;
        } else if (pt1->y < pt2->y) {
                /*
                 * pt1
                 *  |
                 *  |
                 * pt2
                 */
                return DIR_DOWN;
        } else if (pt1->y > pt2->y) {
                /*
                 * pt2
                 *  |
                 *  |
                 * pt1
                 */
                return DIR_UP;
        } else {
                return DIR_UNKNOWN;
        }
}

enum Direction get_corner_dir(const struct Point *pt1, const struct Point *pt2,
                              const struct Point *corner)
{
        static int dir_map[4][4] = {
                /*
                 * CAUTION: make sure DIR_LEFT=0, DIR_RIGHT=1, DIR_UP=2, DIR_DOWN=3
                 **/
                /* DIR_LEFT  DIR_RIGHT  DIR_UP  DIR_DOWN */
                {DIR_LEFT,    DIR_UNKNOWN, DIR_LB,      DIR_LT},  /* DIR_LEFT */
                {DIR_UNKNOWN, DIR_RIGHT,   DIR_RB,      DIR_RT},  /* DIR_RIGHT */
                {DIR_RT,      DIR_LT,      DIR_UP,      DIR_UNKNOWN},  /* DIR_UP */
                {DIR_RB,      DIR_LB,      DIR_UNKNOWN, DIR_DOWN},  /* DIR_DOWN */
        };
        enum Direction dir1, dir2;

        dir1 = get_line_dir(pt1, corner);
        dir2 = get_line_dir(corner, pt2);

        if (!(dir1 >= DIR_LEFT && dir1 <= DIR_DOWN) ||
            !(dir2 >= DIR_LEFT && dir2 <= DIR_DOWN)) {
                return DIR_UNKNOWN;
        }

        return (enum Direction)dir_map[dir1][dir2];
}
