/*
 * A281154  Expansion of (Sum_{k>=2} x^(k^2))^2.
 * https://oeis.org/A281154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (i,j) with i,j >= 2 and i^2 + j^2
 *            = n
 * Range:     u8 n <= 63, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n < 8)                /* minimal sum is 4^2 + 4^2 = 8 */
        return (seq_t)0;

    seq_t res = 0;
    /* maximum i such that i^2 <= n */
    idx_t max_i = (idx_t)floor_sqrt((seq_t)n);

    for (idx_t i = 2; i <= max_i; ++i)
    {
        seq_t i2   = (seq_t)i * (seq_t)i;
        if (i2 > (seq_t)n)        /* safety guard */
            break;

        seq_t j2    = (seq_t)n - i2;
        seq_t sqj   = floor_sqrt(j2);
        if (sqj >= 2 && sqj * sqj == j2)
            ++res;               /* count the ordered pair (i, sqj) */
    }

    return res;
}
