/*
 * A389352  Manhattan distance to the origin of point n while traversing the
 *          half plane by integer points in rectangular layers starting from
 *          n=1 at the origin.
 * https://oeis.org/A389352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |A389649(n)| + A389648(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

/* Computes the 1‑based Manhattan distance of the n‑th point in the
   rectangular‑layer half‑plane traversal. */
seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)0;           /* origin */

    /* binary search for the layer r such that the first r layers
       contain at least n points.  Number of points up to layer r
       (including the origin) is 2*r^2 + 3*r + 1. */
    idx_t lo = 0;
    idx_t hi = 1;
    while (1) {
        __int128 val = (__int128)2 * (__int128)hi * (__int128)hi
                     + (__int128)3 * (__int128)hi + 1;
        if (val >= ( __int128)n)
            break;
        hi <<= 1;   /* double hi - stays within 64‑bit for reasonable n */
    }
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __int128 val = (__int128)2 * (__int128)mid * (__int128)mid
                     + (__int128)3 * (__int128)mid + 1;
        if (val >= ( __int128)n)
            hi = mid;
        else
            lo = mid;
    }
    idx_t r = hi;                      /* layer index (r >= 1) */

    /* points before this layer */
    idx_t r_minus1 = r - 1;
    __int128 prev_val = (__int128)2 * (__int128)r_minus1 * (__int128)r_minus1
                      + (__int128)3 * (__int128)r_minus1 + 1;
    idx_t prevN = (idx_t)prev_val;      /* number of points before layer r */

    idx_t t = n - prevN - (idx_t)1;     /* 0‑based offset inside layer r */

    /* compute coordinates (x,y) along the boundary of the layer */
    int64_t x, y;
    /* the layer consists of
         start: (r,0)
         up side: y = 1..r   (r points)
         top row: x = r-1 .. -r   (2r points)
         left side: y = r-1 .. 0   (r points) */
    if (t == (idx_t)0) {
        x = (int64_t)r; /* (r,0) */
        y = 0;
    } else if (t <= r) {
        x = (int64_t)r;
        y = (int64_t)t;
    } else if (t <= r + 2 * r) {
        idx_t i = t - (r + (idx_t)1);        /* 0‑based along top row */
        x = (int64_t)r - 1 - (int64_t)i;
        y = (int64_t)r;
    } else {
        idx_t u = t - (r + 2 * r + (idx_t)1); /* 0‑based on left side */
        x = -(int64_t)r;
        y = (int64_t)r - 1 - (int64_t)u;
    }

    /* Manhattan distance: |x| + y  (y is never negative in this traversal) */
    seq_t absx = (x >= 0) ? (seq_t)x : (seq_t)(-x);
    seq_t dist = absx + (seq_t)y;
    return dist;
}
