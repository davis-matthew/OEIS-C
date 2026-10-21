/*
 * A249490  a(n) = Sum_{m=0..floor((n-1)/2)} prime((n-m)(n-m-1)/2+m+1).
 * https://oeis.org/A249490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{m=0..floor((n-1)/2)} prime((n-m)(n-m-1)/2 + m + 1)
 * Range:     u8 n <= 7, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(n) retrieving the prime by index using the provided
   get_prime utility, so it works for any seq_t and idx_t defined by
   the build environment. */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset 1: undefined below, return 0 */

    idx_t m_max = (n - 1) / 2;             /* floor((n-1)/2) */
    seq_t sum  = 0;

    for (idx_t m = 0; m <= m_max; ++m) {
        idx_t t   = n - m;
        idx_t pos = (t * (t - 1)) / 2 + m + 1;   /* 1‑based prime index */
        sum += (seq_t)get_prime(pos);             /* get_prime returns uint32_t */
    }
    return sum;
}
