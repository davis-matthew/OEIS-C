/*
 * A332078  Primes p = k*2^m + 1 such that k + 2^m is not prime, where k and m
 *          are the odd part and 2-valuation, respectively, of p-1.
 * https://oeis.org/A332078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p = k*2^m + 1 where k is the odd part of
 *            p‑1, * m = valuation2(p‑1), and k + 2^m is composite
 *            (non‑prime). This is OEIS A332078
 * Range:     u8 n <= 12, u16 n <= 4164, u32 n <= 8651, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, is_prime, is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Utility prototypes (provided by the build system). */
#include "../utils/is_prime.h"           /* bool is_prime(idx_t) */
#include "../utils/is_prime_big.h"      /* bool is_prime_big(seq_t) */
#include "../utils/pow2.h"              /* seq_t pow2(idx_t) */
#include "../utils/ensure_capacity.h"   /* ensure_capacity(size_t, size_t*, void**, size_t) */

static seq_t *cache = NULL;      /* cached sequence values */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static idx_t cache_len = 0;      /* number of terms currently stored */

static idx_t last_checked = 2;   /* last integer examined for primality */

/* Ensure that the cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        /* Find the next prime greater than last_checked. */
        idx_t cand = last_checked + 1;
        while (!is_prime(cand))
            cand++;
        last_checked = cand;

        /* Compute m (valuation of 2 in p-1) and k (odd part of p-1). */
        idx_t temp = cand - 1;
        idx_t m = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            m++;
        }
        seq_t k = (seq_t)temp;          /* odd part of p-1 */
        seq_t pow2m = pow2(m);          /* 2^m */
        seq_t s = k + pow2m;            /* k + 2^m */

        /* If s is composite, store p in the cache. */
        if (!is_prime_big(s)) {
            ensure_capacity(cache_len + 1, &cache_cap,
                            (void **)&cache, sizeof(seq_t));
            cache[cache_len++] = (seq_t)cand;
        }
    }
}

/* OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Undefined for n < 1 */
    ensure_cache(n);
    return cache[n - 1];
}
