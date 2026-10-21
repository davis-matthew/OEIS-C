/*
 * A281158  Least prime p such that n^2 + (n - p)^2 is prime.
 * https://oeis.org/A281158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p such that n^2 + (n - p)^2 is prime
 * Range:     u8 n <= 14, u16 n <= 857, u32 n <= 857, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    /*  n >= 1 is assumed.  */
    idx_t nn   = (idx_t)n;
    const seq_t n_sq = (seq_t)nn * (seq_t)nn;

    uint64_t p = 2;  /* iterate over primes, starting with 2 */
    for (;; ++p) {
        /* test if p is prime */
        if (!is_prime(p))
            continue;

        /* diff = |n - p|  (as idx_t) */
        idx_t diff = ((idx_t)p > nn) ? ((idx_t)p - nn) : (nn - (idx_t)p);

        /* val = n^2 + (n - p)^2 = n_sq + diff^2 */
        seq_t val = n_sq + ((seq_t)diff * (seq_t)diff);

        /* check primality of val */
        if (is_prime_big(val))
            return (seq_t)p;
    }
}
