/*
 * A360821  Number of primes of the form k^2+1 between n^2 and 2*n^2
 *          exclusive.
 * https://oeis.org/A360821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 88, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A360821: number of primes p = k^2+1 with n^2 < p < 2*n^2 */
#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) */
#include "../utils/is_prime_small.h" /* is_prime_small(seq_t) */

seq_t a(idx_t n)
{
    /* n >= 1 for all valid calls */
    idx_t n2   = n * n;                         /* n^2  (within seq_t limits) */
    seq_t upper = (seq_t)2 * n2 - 2;            /* 2*n^2 - 2  */
    idx_t maxk = (idx_t)floor_sqrt(upper);     /* floor( sqrt(2*n^2-2) ) */

    seq_t count = 0;
    for (idx_t k = n; k <= maxk; ++k)
    {
        seq_t p = (seq_t)k * k + 1;            /* k^2 + 1   */
        /* p is automatically < 2*n2 by choice of maxk */
        if (is_prime_small(p))
            ++count;
    }
    return count;
}
