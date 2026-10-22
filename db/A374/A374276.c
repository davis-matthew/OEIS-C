/*
 * A374276  Number of representations of n by the quadratic form x^2 + 3*x*y +
 *          y^2 with 0 <= x <= y.
 * https://oeis.org/A374276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1278, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of representations of n by the quadratic form
 *   x^2 + 3*x*y + y^2   with 0 <= x <= y.
 * The algorithm iterates over all possible y, checks whether the
 * discriminant 5*y*y + 4*n is a perfect square, and if so counts the
 * corresponding x that satisfies the bounds. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    /* upper bound for y is floor(sqrt(n)) */
    seq_t sqrt_n = floor_sqrt((seq_t)n);

    for (idx_t y = 0; (seq_t)y <= sqrt_n; ++y)
    {
        seq_t yy = (seq_t)y;
        seq_t D = 5*yy*yy + 4*(seq_t)n;
        seq_t s = floor_sqrt(D);
        if (s*s != D)               /* discriminant not a perfect square */
            continue;

        /* numerator of the quadratic formula: s - 3*y */
        if (s < 3*yy)               /* would give negative x */
            continue;

        seq_t num = s - 3*yy;
        if (num & 1)                /* not divisible by 2 */
            continue;

        seq_t x = num >> 1;         /* x = num/2 */
        if (x <= yy)
            ++count;
    }

    return count;
}
