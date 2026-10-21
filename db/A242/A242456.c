/*
 * A242456  Least number k such that n!/k is prime.
 * https://oeis.org/A242456
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! / prime(pi(n)) i.e. the product of all integers 1..n
 *            except the largest prime <= n
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* static cache of primes up to the largest n seen so far */
    static uint64_t *prime_cache = NULL;
    static size_t   prime_cnt   = 0;
    static idx_t    prime_limit = 0;

    if (n > prime_limit) {
        if (prime_cache)
            free(prime_cache);
        build_primes_upto(n, &prime_cache, &prime_cnt);
        prime_limit = n;
    }

    /* largest prime <= n is the last element in the cache */
    uint64_t largest = prime_cache[prime_cnt - 1];

    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i) {
        if (i == largest)
            continue;           /* skip the largest prime */
        result *= (seq_t)i;     /* multiply by i */
    }
    return result;
}
