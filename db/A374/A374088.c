/*
 * A374088  Number of ordered pairs (x,y) of positive integers such that x^2 +
 *          x*y + y^2 = n.
 * https://oeis.org/A374088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x,y)∈ℕ^2 | x^2 + x*y + y^2 = n} (ordered pairs)
 * Range:     u8 n <= 16, u16 n <= 3779, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t valN   = (seq_t)n;

    /* Upper bound for x is floor_sqrt(n). */
    idx_t maxX = (idx_t)floor_sqrt(valN);
    for (idx_t x = 1; x <= maxX; ++x)
    {
        seq_t valX   = (seq_t)x;
        seq_t xx     = valX * valX;
        seq_t three_xx = 3 * xx;
        seq_t four_n  = 4 * valN;

        if (three_xx > four_n)          /* discriminant would be negative */
            continue;

        seq_t delta = four_n - three_xx;
        seq_t s     = floor_sqrt(delta);
        if (s * s != delta)             /* not a perfect square */
            continue;

        seq_t diff = s - valX;
        if (diff % 2 != 0)              /* parity condition not met */
            continue;

        seq_t y = diff / 2;
        if (y < valX)                   /* we only process unordered pairs x ≤ y */
            continue;

        if (y == valX)
            count += 1;                 /* diagonal pair (x,x) counts once          */
        else
            count += 2;                 /* (x,y) and (y,x) are distinct ordered pairs */
    }

    return count;
}
