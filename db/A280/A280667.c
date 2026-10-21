/*
 * A280667  a(n) = number of primes of the form 4k + 1 such that 2n - 4k - 1
 *          is prime.
 * https://oeis.org/A280667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of primes p ≡ 1 (mod 4) such that 2n – p is
 *            prime
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Ensure we have the primality test for idx_t integers. */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* The largest prime p that can satisfy 2n – p ≥ 2 is
       p ≤ 2n – 2.  For n = 1 this gives limit = 0. */
    idx_t limit = (n >= 2) ? (2 * n - 2) : 0;

    seq_t count = 0;

    /* Iterate over k ≥ 1, p = 4k + 1. */
    for (idx_t k = 1; ; ++k) {
        idx_t p = 4 * k + 1;
        if (p > limit)
            break;                   /* p cannot satisfy 2n – p ≥ 2   */

        if (!is_prime(p))
            continue;                 /* p must be prime of form 4k+1 */

        idx_t q = 2 * n - p;          /* q = 2n – p  */
        if (q >= 2 && is_prime(q))
            ++count;                  /* both p and q are prime */
    }

    return count;
}
