/*
 * A392377  Odd k such that A393131((k^2-1)/2) =
 *          EulerPhi(k^2)/A329726((k^2-1)/2) is even. In other words, odd k
 *          such that the subgroup of witnesses for Solovay-Strassen primality
 *          test of k^2 has even index in (Z/k^2Z)*.
 * https://oeis.org/A392377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = odd k such that v2(k^2-1) ≤ max{v2(p-1) | p | k}
 * Range:     u8 n <= 9, u16 n <= 6020, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS A392377. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"   /* for factor_u64 */

/* Compute the exponent of 2 in the factorization of x (x > 0). */
static int v2_uint64(uint64_t x)
{
    int c = 0;
    while ((x & 1ULL) == 0ULL) {
        x >>= 1;
        ++c;
    }
    return c;
}

/* Test whether an odd integer k satisfies the required 2‑adic condition. */
static bool satisfies(uint64_t k)
{
    /* v2(k^2-1) = v2(k-1) + v2(k+1). */
    int v2k_1 = v2_uint64(k - 1);
    int v2k1_1 = v2_uint64(k + 1);
    int v2k2_1 = v2k_1 + v2k1_1;

    /* Find the maximal v2(p-1) over all prime factors p of k. */
    uint64_t primes[64], exps[64];
    size_t pcnt;
    factor_u64(k, primes, exps, &pcnt);

    int maxv2 = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        if (p == 2) continue;            /* k is odd, but guard anyway */
        int v = v2_uint64(p - 1);
        if (v > maxv2) maxv2 = v;
    }

    return v2k2_1 <= maxv2;
}

/* Static cache of computed terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 3;   /* next odd number to test */

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, defensive guard */

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Find the next odd k that satisfies the condition. */
        while (true) {
            if (satisfies(next_candidate)) {
                /* Extend the cache array if necessary. */
                if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();      /* allocation failed */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)next_candidate;
                next_candidate += 2;   /* move to the next odd candidate */
                break;
            }
            next_candidate += 2;       /* skip even numbers */
        }
    }

    return cache[n - 1];   /* a(1) is the first element */
}
