/*
 * A377864  Largest divisor of 2n-1 less than or equal to sqrt(2n-1).
 * https://oeis.org/A377864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest divisor of (2n-1) ≤ sqrt(2n-1)
 * Range:     u8 n <= 32, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: brute‑force search descending from floor(sqrt(2n-1)). */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* m = 2n-1 */
    seq_t m = ((seq_t)n * (seq_t)2) - (seq_t)1;

    /* integer square root of m */
    idx_t s = (idx_t)floor_sqrt(m);

    /* search for the largest divisor ≤ s */
    for (idx_t d = s; d; --d)
        if (m % (seq_t)d == (seq_t)0)
            return (seq_t)d;

    /* fallback (will never be reached) */
    return (seq_t)1;
}
