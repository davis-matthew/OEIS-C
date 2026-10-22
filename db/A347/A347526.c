/*
 * A347526  Number of divisors of n that are at most n^(1/4).
 * https://oeis.org/A347526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of n that are at most n^(1/4)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* Count divisors d of n with d ≤ n^(1/4) */
seq_t a(idx_t n)
{
    /* Compute the integer part of the fourth root of n */
    const idx_t r1 = (idx_t)floor_sqrt((seq_t)n);   /* sqrt(n)   */
    const idx_t r4 = (idx_t)floor_sqrt((seq_t)r1);  /* sqrt(sqrt(n)) */

    seq_t count = 0;
    for (idx_t d = 1; d <= r4; ++d) {
        if (n % d == 0) {
            ++count;
        }
    }
    return count;
}
