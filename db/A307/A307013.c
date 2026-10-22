/*
 * A307013  Third coordinate (asymmetric variant) in a redundant hexagonal
 *          coordinate system of the points of a counterclockwise spiral on an
 *          hexagonal grid. First and second coordinates are given in A307011
 *          and A307012.
 * https://oeis.org/A307013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = A307011(n) + A307012(n) Compute a(n) as the sum of the
 *            first two cube‑coordinate components of the counterclockwise
 *            hexagonal spiral
 * Range:     i8 n <= 9991, i16 n <= 9991, i32 n <= 9991, i64 n <= 9991,
 *            i128 n <= 9991, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Hexagonal spiral coordinates.  
   The spiral starts at (0,0,0).  For ring r ≥ 1 the first point
   is (r, -(r-1), -1).  From there we take (r‑1) steps along
   direction (0,1,-1) and then r steps in each of the directions
   (-1,1,0), (-1,0,1), (0,-1,1), (1,-1,0), (1,0,-1).   */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    /* Find the ring r such that n < 1 + 3*r*(r+1)  (total cells up to ring r) */
    idx_t r = 1;
    while (1 + 3 * r * (r + 1) <= n) {
        ++r;
    }

    /* First index of ring r:   T(r-1) = 1 + 3*(r-1)*r */
    idx_t ring_start = 1 + 3 * (r - 1) * r;
    idx_t offset = n - ring_start;   /* 0 … 6r-1   */

    /* Starting cube coordinate for ring r */
    seq_t x = (seq_t)r;
    seq_t y = -(seq_t)(r - 1);
    /* z = -1 is implicit, not needed for the sum */

    if (offset == 0) {
        return x + y;
    }

    /* Directions:
       d5 : (0, 1, -1)            // used (r-1) times from start
       d0 : (-1, 1, 0)
       d1 : (-1, 0, 1)
       d2 : (0, -1, 1)
       d3 : (1, -1, 0)
       d4 : (1, 0, -1)            // used r times each
    */
    static const int dirs[5][3] = {
        {-1,  1, 0},
        {-1,  0, 1},
        { 0, -1, 1},
        { 1, -1, 0},
        { 1,  0, -1}
    };
    static const int d5[3] = {0, 1, -1};

    /* Apply first (r-1) steps in direction d5 */
    idx_t rem = offset;
    idx_t steps_d5 = r > 0 ? r - 1 : 0;

    if (rem <= steps_d5) {
        /* Only part of the d5 steps is needed */
        x += (seq_t)d5[0] * (seq_t)rem;
        y += (seq_t)d5[1] * (seq_t)rem;
        return x + y;
    }

    /* Full d5 steps consumed */
    x += (seq_t)d5[0] * (seq_t)steps_d5;
    y += (seq_t)d5[1] * (seq_t)steps_d5;
    rem -= steps_d5;

    /* Now traverse the remaining sides */
    for (int i = 0; i < 5; ++i) {
        if (rem <= r) {
            x += (seq_t)dirs[i][0] * (seq_t)rem;
            y += (seq_t)dirs[i][1] * (seq_t)rem;
            break;
        } else {
            x += (seq_t)dirs[i][0] * (seq_t)r;
            y += (seq_t)dirs[i][1] * (seq_t)r;
            rem -= r;
        }
    }

    return x + y;
}
