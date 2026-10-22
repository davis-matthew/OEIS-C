/*
 * A336298  Greatest prime < prime(n)/2.
 * https://oeis.org/A336298
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the greatest prime less than prime(n)/2 where prime(n)
 *            is the n-th prime
 * Range:     u8 n <= 54, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   build_primes_upto, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"

/* external linkage */
seq_t a(idx_t n)
{
    /* Prime(n) */
    seq_t p = (seq_t)get_prime(n);
    /* limit = floor(p/2) */
    uint64_t limit = (uint64_t)p / 2U;

    /* Static cache of primes up to current prime_cache_limit */
    static uint64_t *prime_cache = NULL;
    static size_t prime_cache_cnt = 0;
    static uint64_t prime_cache_limit = 0;

    /* If needed, extend the cached prime table */
    if (limit > prime_cache_limit) {
        uint64_t *new_cache = NULL;
        size_t new_cnt = 0;
        /* build_primes_upto creates primes 2..limit inclusive */
        build_primes_upto(limit, &new_cache, &new_cnt);
        free(prime_cache);          /* free old memory if any */
        prime_cache = new_cache;
        prime_cache_cnt = new_cnt;
        prime_cache_limit = limit;
    }

    /* Binary search for the largest prime <= limit */
    size_t left = 0, right = prime_cache_cnt;
    while (left < right) {
        size_t mid = (left + right) >> 1;
        if (prime_cache[mid] <= limit)
            left = mid + 1;
        else
            right = mid;
    }
    size_t idx = (left == 0) ? 0 : left - 1;

    return (seq_t)prime_cache[idx];
}
