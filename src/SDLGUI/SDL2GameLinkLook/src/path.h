#ifndef PATH_H
#define PATH_H

#include "map.h"

enum PathType {
        PATH_ZERO_CORNER,
        PATH_ONE_CORNER,
        PATH_TWO_CORNER,
        PATH_NOT_FOUND
};

/*
 * Is there a valid path between pt1 and pt2 on the map.
 *
 * A valid path means there is no barrier tile on the path.
 *
 * if PATH_ZERO_CORNER is returned, None of corner1 and corner2 is set;
 * if PATH_ONE_CORNER is returned, corner1 is set and corner2 is not set;
 * if PATH_TWO_CORNER is returned, both of corner1 and corner2 is set;
 */
enum PathType find_path(const struct Map *map,
                        const struct Point *pt1, const struct Point *pt2,
                        struct Point *corner1, struct Point *corner2);

#endif /* PATH_H */
