#ifndef POINT_H
#define POINT_H

struct Point {
        int x;
        int y;
};

#define EMPTY_POINT(pt) \
        pt.x = -1, pt.y = -1

#define POINT_IS_NULL(pt) \
        (pt.x == -1 && pt.y == -1)

#define POINT_SET(pt, _x, _y) \
        (pt).x = _x, (pt).y = _y

#define POINT_EQ(pt1, pt2) \
        ((pt1.x == pt2.x) && (pt1.y == pt2.y))

enum Direction {
        DIR_LEFT = 0,
        DIR_RIGHT = 1,
        DIR_UP = 2,
        DIR_DOWN = 3,
        DIR_LT, /* Left-Top */
        DIR_RT, /* Right-Top */
        DIR_LB, /* Left-Bottom */
        DIR_RB, /* Right-Bottom */
        DIR_UNKNOWN,
};

enum Direction get_line_dir(const struct Point *pt1, const struct Point *pt2);

enum Direction get_corner_dir(const struct Point *pt1, const struct Point *pt2,
                              const struct Point *corner);

#endif /* POINT_H */
