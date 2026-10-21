/*
 * A259978  Terms in A053177 that are relatively prime to 3.
 * https://oeis.org/A259978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*A275770(n)+1. A275770(n) are primes p ≡ 2 (mod 3) for
 *            which 2p+1 is composite. The sequence consists of a(n) = 2p+1.
 *            This function generates the required values lazily; it is safe
 *            for calls in arbitrary order
 * Range:     u8 n <= 6, u16 n <= 1293, u32 n <= 4332, u64 n <= 4332,
 *            u128 n <= 4332, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Test whether n is prime (simple deterministic test,
   sufficient for n < 2^64 in this application). */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1ULL) == 0 || n % 3ULL == 0) return false;
    for (uint64_t i = 5ULL; i * i <= n; i += 6ULL) {
        if (n % i == 0 || n % (i + 2ULL) == 0) return false;
    }
    return true;
}

/* Sequence cache */
static seq_t *cache      = NULL;   /* array of a(n) values */
static size_t cache_sz   = 0;      /* how many terms are in cache */
static size_t cache_cap  = 0;      /* allocated capacity */

/* Last number that was examined as a candidate prime p. */
static uint64_t last_checked = 1;  /* start from 2 on first call */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety – the sequence is 1‑based */

    /* grow cache until it contains at least n terms */
    while (cache_sz < (size_t)n) {
        ++last_checked;                      /* candidate p */

        /* check congruence first – fast reject */
        if ((last_checked % 3ULL) != 2ULL) continue;

        /* is p prime? */
        if (!is_prime_uint64(last_checked)) continue;

        /* a = 2p + 1 */
        uint64_t a_val = 2ULL * last_checked + 1ULL;

        /* keep only composites a */
        if (is_prime_uint64(a_val)) continue;

        /* store term */
        if (cache_sz == cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 8;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_sz++] = (seq_t)a_val;
    }

    return cache[(size_t)n - 1];
}
