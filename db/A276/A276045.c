/*
 * A276045  Primes p such that d(p*(2p+1)) = 8 where d(n) is the number of
 *          divisors of n (A000005).
 * https://oeis.org/A276045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that d(p*(2p+1)) = 8
 * Range:     u8 n <= 19, u16 n <= 1592, u32 n <= 1812, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/ensure_capacity.h"

/* Memoisation state */
static seq_t *cache = NULL;        /* array of found primes */
static idx_t cache_len = 0;        /* number of primes stored */
static size_t cache_cap = 0;       /* allocated capacity of cache */
static uint64_t last_checked = 1; /* last integer examined */

/* Ensure the cache can hold at least 'needed' elements */
static void _grow_cache(idx_t needed)
{
    if (cache_len >= needed) return;
    if (needed > cache_cap) {
        /* Grow to at least double the current capacity or the needed size */
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        ensure_capacity(new_cap, &cache_cap, (void **)&cache, sizeof(seq_t));
    }
}

/* Generate primes until we have at least n terms in cache */
static void _ensure_cache(idx_t n)
{
    uint64_t candidate;
    for (candidate = last_checked + 1; cache_len < n; ++candidate) {
        /* Skip even numbers after 2 */
        if (candidate > 2 && (candidate & 1ULL) == 0) continue;

        /* Test primality */
        if (!is_prime_big((seq_t)candidate)) continue;

        /* 2p+1 must have exactly 4 divisors */
        uint64_t m = 2 * candidate + 1U;
        if (divisor_count_u64(m) == 4) {
            /* Store the prime */
            _grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)candidate;
        }
        /* Remember where we left off */
        last_checked = candidate;
    }
    /* Adjust last_checked for the next call */
    last_checked = candidate - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* offset starts at 1 */
    if (n > cache_len) _ensure_cache(n);
    return cache[n - 1];
}
