/*
 * A368643  Manhattan distance from the origin of triangular number point T(n)
 *          = n*(n+1)/2 in a square spiral with point 0 at the origin.
 * https://oeis.org/A368643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A214526( n*(n+1)/2 + 1 ) Computes the Manhattan distance
 *            from the origin of the point numbered n*(n+1)/2 in a square
 *            spiral that starts at 1. The spiral numbering is: 1 at the
 *            origin, then move right, up, left, down, etc. The function uses
 *            only integer arithmetic and the utility floor_sqrt...
 * Range:     u8 n <= 10, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

static inline seq_t abs_diff(seq_t a, seq_t b)
{
    return (a > b) ? (a - b) : (b - a);
}

seq_t a(idx_t n)
{
    /* Triangular number point plus one so it matches A214526's 1‑based index. */
    seq_t tn = (seq_t)n * ((seq_t)n + 1) / 2 + 1;

    /* 1 corresponds to the origin. */
    if (tn <= 1) return 0;

    /* Compute the odd square root ceiling of tn. */
    seq_t s = floor_sqrt(tn);          /* floor(sqrt(tn)) */
    if (s * s < tn) s++;               /* ceil(sqrt(tn))     */
    if ((s & 1) == 0) s++;             /* make it odd        */

    /* Layer radius */
    seq_t m = (s - 1) / 2;

    /* Previous layer's maximum value: (s-2)^2 */
    seq_t prev_max = (s - 2) * (s - 2);

    /* Offset within the current ring, zero‑based. */
    seq_t offset = tn - prev_max - 1;
    seq_t side_len = 2 * m;          /* each side has 2m points */

    seq_t dist;
    if (offset < side_len) {                   /* right side            */
        dist = m + abs_diff(offset, m - 1);
    } else if (offset < side_len * 2) {          /* top side              */
        seq_t t = offset - side_len;
        dist = m + abs_diff(t + 1, m);
    } else if (offset < side_len * 3) {          /* left side             */
        seq_t t = offset - side_len * 2;
        dist = m + abs_diff(t + 1, m);
    } else {                                    /* bottom side           */
        seq_t t = offset - side_len * 3;
        dist = m + abs_diff(t + 1, m);
    }

    return dist;
}
