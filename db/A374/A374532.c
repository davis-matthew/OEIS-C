/*
 * A374532  Number of complete unit squares that fit inside a circle of radius
 *          sqrt(n^2+1) centered at the origin of a square lattice.
 * https://oeis.org/A374532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*A237526(n²+1)
 * Range:     u8 n <= 9, u16 n <= 145, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts complete unit squares inside a circle of radius sqrt(n²+1). */

#include <stdint.h>

/* Integer square‑root: floor(sqrt(n)) for 0 <= n <= 2⁶⁴‑1. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t x = 0;
    idx_t b = (idx_t)1 << 62;           /* highest power of 4 <= n */
    while (b > 0) {
        if (n >= x + b) {
            n -= x + b;
            x = (x >> 1) + b;
        } else {
            x >>= 1;
        }
        b >>= 2;
    }
    return x;
}

seq_t a(idx_t n)
{
    /* Radius squared : n² + 1.  All values fit in 64 bits for n <= 10000. */
    idx_t m = (idx_t)n * (idx_t)n + 1;

    /* Count lattice points (x,y) with x>=1, y>=1 and x² + y² <= m. */
    seq_t count = 0;
    idx_t max_x = isqrt_u64(m);

    for (idx_t x = 1; x <= max_x; ++x) {
        idx_t rem = m - (idx_t)x * (idx_t)x;       /* remaining radius² for y² */
        idx_t max_y = isqrt_u64(rem);
        count += (seq_t)max_y;
    }

    return (seq_t)4 * count;   /* all four quadrants */
}
