/*
 * A268038  List of y-coordinates of point moving in clockwise square spiral.
 * https://oeis.org/A268038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = y‑coordinate of point moving in a clockwise square spiral,
   starting at the origin (0,0).  The first few terms are:
   0, 0, -1, -1, -1, 0, 1, 1, 1, 1, 0, -1, -2, -2, -2, -2, -2, -1, 0, 1, 2, 2, 2, 2, … */

seq_t a(idx_t n) {
    /* The origin is term 1. */
    if (n <= 1) return 0;

    idx_t steps = n - 1;        /* number of moves after the origin */
    seq_t y = 0;                /* current y‑coordinate */

    /* pair is the current side length of the square spiral.
       For pair = 1, the directions are east (0) followed by south (1).
       For pair = 2, the directions are west (2) followed by north (3), etc. */
    idx_t pair = 1;
    int dir1 = 0;  /* first direction of the pair */
    int dir2 = 1;  /* second direction of the pair */

    while (steps > 0) {
        /* First segment of the current pair */
        idx_t d = pair;
        if (d > steps) d = steps;
        if (dir1 == 1) y -= (seq_t)d;   /* south */
        else if (dir1 == 3) y += (seq_t)d; /* north */
        steps -= d;
        if (steps == 0) break;

        /* Second segment of the current pair */
        d = pair;
        if (d > steps) d = steps;
        if (dir2 == 1) y -= (seq_t)d;   /* south */
        else if (dir2 == 3) y += (seq_t)d; /* north */
        steps -= d;
        if (steps == 0) break;

        ++pair;                          /* move to the next layer length */

        /* Alternate the direction pair: even pair -> west/north,
           odd pair  -> east/south */
        if (pair % 2 == 0) {
            dir1 = 2;   /* west  */
            dir2 = 3;   /* north */
        } else {
            dir1 = 0;   /* east  */
            dir2 = 1;   /* south */
        }
    }

    return y;
}
