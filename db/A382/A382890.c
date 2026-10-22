/*
 * A382890  The square root of the largest square dividing the n-th cubefree
 *          number.
 * https://oeis.org/A382890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sqrt of the largest square dividing the n-th cubefree
 *            number (equivalently, the product of the primes that occur with
 *            exponent 2 in that number)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static storage for previously computed values */
static seq_t *cache = NULL;      /* array of a(k) values (1‑based) */
static size_t cache_len = 0;     /* number of values stored */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static uint64_t next_candidate = 1;   /* next integer to test for cubefreeness */

/* Ensure that cache contains at least n terms (n >= 1). */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n)
        return;

    if (!cache) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    while (cache_len < n) {
        uint64_t m = next_candidate;
        seq_t t = 1;          /* product of primes with exponent 2 */
        bool cubefree = true;
        uint64_t temp = m;

        for (uint64_t p = 2; p * p <= temp; ++p) {
            if (temp % p == 0) {
                int exp = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++exp;
                }
                if (exp == 2) {
                    t *= (seq_t)p;
                } else if (exp >= 3) {
                    cubefree = false;
                    break;
                }
            }
        }

        if (cubefree) {
            /* temp > 1 corresponds to a prime factor of exponent 1 – ignore */
            if (cache_len >= cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = t;
        }
        ++next_candidate;
    }
}

/* Public sequence accessor */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined, but guard against zero index */

    ensure_cache(n);
    return cache[n - 1];    /* cache is zero‑indexed */
}
