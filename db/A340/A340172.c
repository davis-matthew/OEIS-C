/*
 * A340172  List of Y-coordinates of point moving along one of the arms of a
 *          counterclockwise double square spiral; A340171 gives
 *          X-coordinates.
 * https://oeis.org/A340172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the y‑coordinate of the double square spiral, where the
 *            step lengths are 1,1,3,3,5,5,7,7,…. The spiral starts at (0,0)
 *            and moves east, north, west, south, repeatedly, increasing the
 *            step length by 2 after completing a northward or southward
 *            segment
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t x = 0, y = 0;             /* current coordinates          */
    idx_t dir    = 0;                /* 0:east, 1:north, 2:west, 3:south */
    idx_t len    = 1;                /* current side length          */
    idx_t steps  = len;              /* steps remaining in this dir  */

    for (idx_t i = 1; i <= n; ++i) {
        /* move one step in the current direction */
        if (dir == 0)  ++x;
        else if (dir == 1)  ++y;
        else if (dir == 2)  --x;
        else                --y;   /* dir == 3 */

        --steps;

        /* finished this segment */
        if (steps == 0) {
            dir = (dir + 1) & 3;          /* cycle 0→1→2→3→0 …           */
            if (dir == 0 || dir == 2)    /* after west or south          */
                len += 2;                /* increase step length         */
            steps = len;
        }
    }

    return y;     /* y-coordinate at index n */
}
