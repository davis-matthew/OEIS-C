/*
 * A398353  Least prime p such that n - p is semiprime or 0 if no such prime
 *          exists.
 * https://oeis.org/A398353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least prime p such that n - p is semiprime (product of 2
 *            primes), 0 if no such prime exists
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   _smallest_prime_factor, build_primes_upto, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/_smallest_prime_factor.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* static cache of primes up to the maximum queried n */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_limit = 0;

    if (n < 1) return 0;          /* undefined for n < 1, return 0 */

    /* Ensure the prime table is large enough */
    if (n > prime_limit) {
        /* free old array if it exists */
        if (primes) free(primes);
        build_primes_upto(n, &primes, &prime_cnt);
        prime_limit = n;
    }

    /* iterate primes in ascending order to find the smallest p */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > n) break;

        seq_t rem = (seq_t)(n - p);
        if (rem < 2) continue;  /* no semiprime smaller than 4 */

        /* check if rem is semiprime: product of two primes */
        uint64_t spf = _smallest_prime_factor((uint64_t)rem);
        if (spf == 0 || spf == (uint64_t)rem) continue;  /* 0 or prime -> not semiprime */

        uint64_t second = (uint64_t)rem / spf;
        if (is_prime_small((seq_t)second)) {
            /* rem = spf * second and both primes -> semiprime */
            return (seq_t)p;
        }
    }

    /* no suitable prime found */
    return 0;
}
