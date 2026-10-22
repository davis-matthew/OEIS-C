/*
 * A345161  If n = Product (p_j^k_j) then a(n) = max (nextprime(p_j) - p_j),
 *          where nextprime = A151800.
 * https://oeis.org/A345161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max( nextprime(p) - p ) over all distinct prime divisors
 *            p of n (the maximum gap to the next prime). a(1) is defined to
 *            be 1
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/next_prime.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n == 1)                      /* a(1) = 1 */
        return (seq_t)1;

    uint64_t remaining = (uint64_t)n;
    seq_t     max_gap   = 0;

    /* Primes up to sqrt(remaining).  Static cache reused for subsequent calls. */
    static uint64_t *prime_cache = NULL;
    static size_t   prime_cnt    = 0;
    static uint64_t prime_cache_max = 0;

    uint64_t limit = (uint64_t)_floor_isqrt_u64(remaining);
    if (limit > prime_cache_max) {
        /* Rebuild cache with a larger limit. */
        if (prime_cache) free(prime_cache);
        build_primes_upto(limit, &prime_cache, &prime_cnt);
        prime_cache_max = limit;
    }

    /* Iterate over the cached primes.  Remaining may shrink during factoring. */
    size_t i = 0;
    while (i < prime_cnt && prime_cache[i] * prime_cache[i] <= remaining) {
        uint64_t p = prime_cache[i];
        if (remaining % p == 0) {
            seq_t gap = (seq_t)(next_prime(p) - p);
            if (gap > max_gap) max_gap = gap;
            /* Remove all powers of p. */
            do { remaining /= p; } while (remaining % p == 0);
        }
        ++i;
    }

    /* If remaining > 1, then it itself is a prime divisor. */
    if (remaining > 1) {
        seq_t gap = (seq_t)(next_prime(remaining) - remaining);
        if (gap > max_gap) max_gap = gap;
    }

    return max_gap;
}
