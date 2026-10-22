/*
 * A330300  a(n) is the number of subsets of {1..n} that contain exactly 2 odd
 *          and 3 even numbers.
 * https://oeis.org/A330300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(ceil(n/2), 2) * binomial(floor(n/2), 3)
 * Range:     u8 n <= 11, u16 n <= 31, u32 n <= 279, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t odd   = (n + 1) / 2;   /* ceil(n/2) */
    idx_t even  = n / 2;        /* floor(n/2) */

    if (odd < 2 || even < 3)
        return 0;

    seq_t res = binomial(odd, 2);
    res *= binomial(even, 3);
    return res;
}
