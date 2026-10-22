/*
 * A374092  Number of solutions to n = x^2 + x*y + y^2 with 0 < x < y.
 * https://oeis.org/A374092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (x,y) : 0 < x < y and n = x^2 + x*y + y^2 }
 * Range:     u8 n <= 112, u16 n <= 112, u32 n <= 112, u64 n <= 112,
 *            u128 n <= 112, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm iterates over x, checks the quadratic condition using an
   integer square root, and counts the valid pairs. It runs in
   O(√n) time. */

#include <stdbool.h>

/* Integer square root for idx_t, implemented by binary search. */
static idx_t isqrt_idx(idx_t v) {
    idx_t lo = 0, hi = v;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        idx_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v)  lo = mid + 1;
        else         hi = mid - 1;
    }
    return hi;
}

/* Count the solutions of n = x^2 + x*y + y^2 with 0 < x < y. */
seq_t a(idx_t n) {
    seq_t count = 0;
    idx_t nn = n;
    idx_t limit = 4 * nn;                /* limit = 4n  */

    for (idx_t x = 1; ; ++x) {
        idx_t xx3 = 3 * x * x;            /* 3*x^2  */
        if (xx3 > limit) break;          /* stop when 3x^2 > 4n */

        idx_t t = limit - xx3;           /* t = 4n - 3x^2 */
        idx_t s = isqrt_idx(t);          /* s = floor(sqrt(t)) */
        if (s * s != t) continue;        /* t must be a perfect square */

        if (s <= x) continue;            /* ensure s > x  ->  y > 0   */
        if (((s - x) & 1) != 0) continue;/* (s - x) must be even */

        idx_t y = (s - x) >> 1;          /* y = (s - x)/2   */
        if (y > x) ++count;              /* enforce 0 < x < y */
    }

    return count;
}
