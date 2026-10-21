/*
 * A297177  Rational primes that decompose in the field Q(sqrt(23)).
 * https://oeis.org/A297177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth rational prime that splits in Q(√23) * i.e. the
 *            nth prime p (p ≠ 23) for which the Legendre symbol * (23|p) = 1.
 *            The first terms are 7, 11, 13, 19, …
 * Range:     u8 n <= 22, u16 n <= 3254, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/grow_seq.h"

/* 64‑bit modular exponentiation (Euler criterion). */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp != 0) {
        if (exp & 1ULL)
            result = (uint64_t)((__int128)result * base % mod);
        exp >>= 1ULL;
        if (exp)
            base = (uint64_t)((__int128)base * base % mod);
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Memoized list of splitting primes. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;    /* number of primes stored */
    static size_t cache_cap = 0;     /* allocated capacity */
    static uint64_t current = 1;     /* last odd number tested */

    while (cache_size < (size_t)n) {
        current += 2;                            /* next odd candidate   */
        if (current == 23) continue;             /* skip the ramified prime */
        if (!is_prime(current)) continue;        /* enforce primality    */
        /* Euler's criterion for Legendre symbol (23|p). */
        if (mod_pow_uint64(92, (current - 1ULL) / 2ULL, current) != 1ULL)
            continue;                            /* not a splitting prime */

        /* Grow the cache if needed.  grow_seq expands the array
           once the capacity is exhausted. */
        if (cache_size == cache_cap)
            grow_seq(&cache, &cache_cap, cache_size + 1);

        cache[cache_size++] = (seq_t)current;     /* store the prime     */
    }

    return cache[n - 1];                         /* zero‑based indexing */
}
