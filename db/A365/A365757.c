/*
 * A365757  G.f. satisfies A(x) = 1 + x*A(x) / (1 - x^3*A(x)^5).
 * https://oeis.org/A365757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} binomial(n-2k-1, k) * binomial(n+2k+1,
 *            n-3k) / (n+2k+1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* Ensure the cache has room for indices up to n (inclusive). */
static void ensure_cache(idx_t n, seq_t **cache, size_t *cap)
{
    if (n < *cap) return;
    size_t new_cap = *cap ? *cap : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    *cache = tmp;
    *cap   = new_cap;
}

/* Public entry point */
seq_t a(idx_t n)
{
    /* Static cache for memoization. */
    static seq_t *cache   = NULL;
    static idx_t cached_n = 0;
    static size_t cache_cap = 0;

    /* First call: allocate cache[0] = 1. */
    if (cache == NULL) {
        cache   = (seq_t *)calloc(1, sizeof(seq_t));
        cache[0] = 1;
        cache_cap = 1;
        cached_n = 0;
    }

    /* a(0) is defined as 1. */
    if (n == 0) return 1;

    /* Extend cache if needed. */
    if (n > cached_n) {
        ensure_cache(n, &cache, &cache_cap);

        for (idx_t i = cached_n + 1; i <= n; ++i) {
            seq_t sum = 0;
            idx_t kmax = i / 3;
            for (idx_t k = 0; k <= kmax; ++k) {
                idx_t t1 = i - 2 * k - 1;            /* n - 2k - 1 */
                idx_t t2 = i + 2 * k + 1;            /* n + 2k + 1 */

                /* Use provided binomial routine. */
                seq_t b1 = binomial(t1, k);
                seq_t b2 = binomial(t2, i - 3 * k);

                seq_t prod = b1 * b2;
                prod = prod / (seq_t)t2;           /* division is exact */
                sum += prod;
            }
            cache[i] = sum;
        }
        cached_n = n;
    }

    return cache[n];
}
