/*
 * A280317  Ordinate of points (x,y) of the square lattice such that x >= 0
 *          and 0 <= y <= x, and ranked in order of increasing distance from
 *          the origin. Equidistant points are ranked in order of increasing
 *          ordinate.
 * https://oeis.org/A280317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A280317: ordinate of n-th lattice point (x,y) with 0 <= y <= x,
 * sorted by increasing Euclidean distance sqrt(x^2 + y^2), ties broken by
 * increasing y. */

#include <stdint.h>

/* Integer square root: floor(sqrt(v)) for an unsigned 64‑bit integer. */
static idx_t isqrt_u64(idx_t v)
{
    if (v == 0) return 0;
    idx_t bit = 1ULL << 62;              /* highest power of 4 <= 2^64 */
    while (bit > v) bit >>= 2;           /* shift until bit <= v */
    idx_t r = 0;
    while (bit != 0) {
        if (v >= r + bit) {
            v -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Return a(n) as defined for OEIS A280317. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* index 0 not used, but safe */
    idx_t count = 0;

    for (idx_t d = 0; ; ++d) {
        /* For distance squared d, find all (x,y) with x^2 + y^2 = d,
           0 <= y <= x.  Iterate y in increasing order to enforce tie
           breaking by ordinate.  Since y <= x, we have y^2 <= d/2. */
        idx_t maxy = isqrt_u64(d >> 1);
        for (idx_t y = 0; y <= maxy; ++y) {
            idx_t rem = d - y * y;
            idx_t x   = isqrt_u64(rem);
            if (x * x != rem) continue;    /* not a perfect square */
            if (x < y) continue;           /* should not happen, safeguard */

            ++count;
            if (count == n) return (seq_t)y;
        }
    }
}
