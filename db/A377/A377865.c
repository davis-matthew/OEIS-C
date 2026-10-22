/*
 * A377865  Smallest divisor of 2n-1 greater than or equal to sqrt(2n-1).
 * https://oeis.org/A377865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest divisor of 2n-1 that is ≥ sqrt(2n-1)
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t m = ((seq_t)n) * ((seq_t)2) - ((seq_t)1);   /* 2n-1 */
    seq_t s = floor_sqrt(m);                          /* floor(sqrt(m)) */

    for (seq_t d = s; d; --d)                         /* search largest d ≤ √m */
    {
        if (m % d == 0)
            return m / d;                             /* smallest divisor ≥ √m */
    }
    return m; /* should never occur: fallback to m itself */
}
