/*
 * A343478  Prime numbers p == 2 (mod 3) such that p-1 has exactly one
 *          distinct odd prime divisor and p+1 has exactly one distinct prime
 *          divisor > 3.
 * https://oeis.org/A343478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 400, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A343478: primes p ≡ 2 (mod 3) such that
 *   - the odd part of p−1 has exactly one distinct prime divisor
 *   - the part of p+1 remaining after removing powers of 2 and 3
 *     has exactly one distinct prime divisor larger than 3
 *
 * The function generates primes in ascending order and caches the
 * terms that satisfy the condition.  It works for any seq_t type
 * (uint8_t … uint64_t, unsigned __int128, or an arbitrary‑precision
 * integer class) as long as seq_t can be constructed from a uint64_t
 * by static_cast.  No global state other than the cache is used.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/next_prime.h"
#include "../utils/count_distinct_primes.h"

/* Helper: test whether a prime p satisfies the sequence conditions. */
static bool prime_good(uint64_t p)
{
    /* p must be 2 mod 3 */
    if (p % 3U != 2U)
        return false;

    /* p-1: strip powers of 2, then check that the remaining odd part
     * has exactly one distinct prime divisor. */
    uint64_t odd = p - 1ULL;
    while ((odd & 1ULL) == 0ULL)      /* remove 2's */
        odd >>= 1ULL;
    if (odd <= 1ULL || count_distinct_primes(odd) != 1U)
        return false;

    /* p+1: first remove powers of 2, then powers of 3.
     * The remaining part must have exactly one distinct prime divisor.
     */
    uint64_t rest = p + 1ULL;
    while ((rest & 1ULL) == 0ULL)      /* remove 2's */
        rest >>= 1ULL;
    while (rest % 3ULL == 0ULL)        /* remove 3's */
        rest /= 3ULL;
    if (rest <= 1ULL || count_distinct_primes(rest) != 1U)
        return false;

    return true;
}

/* Return the nth term of the sequence (1‑based).  The function
 * is re‑entrant and works for any order of calls.  It caches
 * previously found terms for efficiency. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset is 1; not part of the sequence */

    /* Static cache of previously found terms. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static uint64_t last_checked = 1ULL;   /* start before the first prime */

    /* Return cached value if already computed. */
    if ((size_t)n <= cache_size)
        return cache[(size_t)n - 1];

    /* Initialise cache capacity. */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();
    }

    /* Generate successive primes until we have the nth term. */
    while (cache_size < (size_t)n) {
        uint64_t p = next_prime(last_checked + 1ULL);
        last_checked = p;
        if (prime_good(p)) {
            if (cache_size == cache_cap) {      /* grow cache */
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache)
                    abort();
            }
            cache[cache_size++] = (seq_t)p;
        }
    }

    return cache[(size_t)n - 1];
}
