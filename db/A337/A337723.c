/*
 * A337723  a(n) = prime(n-2) - ceiling(a(n-2)/2); a(1)=0, a(2)=1.
 * https://oeis.org/A337723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n-2) - ceil(a(n-2)/2) a(1) = 0, a(2) = 1 The
 *            function memoises all computed terms and generates primes on
 *            demand
 * Range:     u8 n <= 77, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility functions */
#include "../utils/is_prime.h"      /* bool is_prime(idx_t) */
#include "../utils/seq_alloc.h"    /* seq_t *seq_alloc(size_t) */
#include "../utils/seq_realloc.h" /* seq_t *seq_realloc(seq_t*, size_t, size_t) */

/* Global cache for a(n) */
static seq_t *cache = NULL;   /* 1‑based indices; cache[0] unused */
static size_t cache_len = 0;  /* highest n stored */

/* Prime list (0‑based index) */
static uint64_t *primes = NULL;
static size_t primes_len = 0;
static size_t primes_cap = 0;

/* Ensure at least k primes are stored (k >= 0).  prime(k-1) is requested
   when k >= 1. */
static void ensure_primes_up_to(size_t k)
{
    if (primes_len >= k) return;

    if (!primes) {
        primes_cap = 16;
        primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
        if (!primes) exit(1); /* out of memory */
    }

    uint64_t candidate = (primes_len == 0) ? 2 : primes[primes_len - 1] + 1;

    while (primes_len < k) {
        while (!is_prime((idx_t)candidate)) ++candidate;

        if (primes_len >= primes_cap) {
            primes_cap <<= 1;
            uint64_t *tmp = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
            if (!tmp) exit(1);
            primes = tmp;
        }
        primes[primes_len++] = candidate;
        ++candidate;
    }
}

/* Ensure cache contains a(i) for all 1 <= i <= n. */
static void ensure_cache_up_to(idx_t n)
{
    if (cache_len >= (size_t)n) return;

    size_t old_len = cache_len;
    size_t new_cap = (size_t)n + 1;  /* include index 0 for simplicity */

    if (!cache) {
        cache = seq_alloc(new_cap);
        if (!cache) exit(1);
    } else {
        seq_t *tmp = seq_realloc(cache, old_len + 1, new_cap);
        if (!tmp) exit(1);
        cache = tmp;
    }
    cache_len = (size_t)n;

    for (idx_t i = (idx_t)(old_len + 1); i <= n; ++i) {
        if (i == 1) { cache[1] = (seq_t)0; continue; }
        if (i == 2) { cache[2] = (seq_t)1; continue; }

        seq_t prev = cache[(size_t)i - 2];
        seq_t ceil_half = (prev >> 1) + (prev & (seq_t)1);

        /* prime(i-2) => need at least (i-2) primes (0‑based index i-3) */
        ensure_primes_up_to((size_t)(i - 2));
        seq_t prime_val = (seq_t)primes[(size_t)(i - 3)];

        cache[(size_t)i] = prime_val - ceil_half;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n < 1, but safe fallback */
    ensure_cache_up_to(n);
    return cache[(size_t)n];
}
