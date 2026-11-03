/*
 * A339466  Primes p such that gpf((p - 1)/gpf(p - 1)) > 3, where gpf(m) is
 *          the greatest prime factor of m, A006530.
 * https://oeis.org/A339466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 4202, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9808 (2026-11-03)
 * Helpers:   largest_prime_factor, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A339466: primes p such that gpf((p-1)/gpf(p-1)) > 3.
 * The function a(idx_t n) generates primes incrementally until
 * the nth term is found.  Memoization is used so that subsequent
 * calls (in any order) can reuse previously computed terms.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/largest_prime_factor.h"
#include "../utils/next_prime.h"
#include "../utils/mem.h"

/* External linkage function as required */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static uint64_t last_prime = 1;        /* largest prime already tested */

    /* if we already have the term, return it */
    if (n <= cache_size) {
        return cache[n - 1];
    }

    /* ensure the cache can grow */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) return 0; /* allocation failure, but unlikely in tests */
    }

    /* expand until we have nth term */
    while (cache_size < (size_t)n) {
        /* get next prime after last_prime */
        last_prime = next_prime(last_prime);
        uint64_t m = last_prime - 1;

        /* skip if m==0 (should not happen) */
        if (m == 0) continue;

        uint64_t g1 = largest_prime_factor(m);        /* greatest prime factor of m */
        if (g1 == 0) continue;      /* should not happen */

        uint64_t q = m / g1;         /* quotient */

        /* check the condition: gpf(q) > 3 */
        if (largest_prime_factor(q) > 3) {
            /* store the prime, cast to seq_t (assumed to fit) */
            if (cache_size >= cache_cap) {
                cache_cap *= 2;
                cache = mem_resize(seq_t, cache, cache_cap);
                if (!cache) return 0;
            }
            cache[cache_size] = (seq_t)last_prime;
            cache_size++;
        }
    }

    return cache[n - 1];
}
