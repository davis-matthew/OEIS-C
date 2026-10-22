/*
 * A366519  Largest odd divisor of n which is <= sqrt(n).
 * https://oeis.org/A366519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest odd divisor of n that is <= sqrt(n)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n >= 1 as per OEIS offset */
    seq_t nn = (seq_t)n;                    /* work in seq_t to avoid overflow */
    seq_t sq = floor_sqrt(nn);              /* floor(sqrt(n)) */

    /* Search downwards for the largest odd divisor <= sqrt(n) */
    for (seq_t d = sq; d > 0; --d)
    {
        if ((d & 1) == 1 && (nn % d) == 0)
            return d;
    }

    /* The loop always finds at least 1, but the return here
       satisfies the compiler for completeness. */
    return 1;
}
