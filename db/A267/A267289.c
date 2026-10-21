/*
 * A267289  Primes p such that (2^k)+p+(2^(k+1)) is also prime, where 2^k is
 *          the largest power of 2 smaller than p.
 * https://oeis.org/A267289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime p such that p + 3*2^k is also prime,
 *            where 2^k is the largest power of 2 smaller than p
 * Range:     u8 n <= 10, u16 n <= 886, u32 n <= 192, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64, is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/floor_log2_uint64.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached sequence values          */
    static size_t cache_cap = 0;     /* capacity of the cache          */
    static size_t cache_len = 0;     /* number of entries in the cache */
    static idx_t last_prime = 3;     /* last prime examined (start < 5) */

    /* Grow the cache if it does not contain n terms yet */
    if (n > cache_len) {
        /* Initialise cache if it is empty */
        if (cache_cap == 0) {
            cache_cap = 32;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        }

        while (cache_len < n) {
            /* Find the next prime after last_prime */
            last_prime = next_prime(last_prime);
            seq_t p = (seq_t)last_prime;

            /* Compute k = floor(log2(p-1)) */
            idx_t k = floor_log2_uint64((idx_t)(p - 1));

            /* Compute 2^k */
            seq_t pow2k = (seq_t)1 << k;

            /* Compute q = p + 3*2^k */
            seq_t q = p + 3 * pow2k;

            /* If q is prime, store p */
            if (is_prime_big(q)) {
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = p;
            }
        }
    }

    return cache[n - 1];
}
