/*
 * A386300  a(n) = 1 + Sum_{k=0..n-1} (-1)^k * binomial(n-1,k) * a(k) *
 *          a(n-1-k).
 * https://oeis.org/A386300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 + Σ_{k=0}^{n-1} (-1)^k * binomial(n-1,k) * a(k) *
 *            a(n-1-k) OEIS offset 0 (a(0)=1). Only even indices equal 1; all
 *            odd indices are computed from the recurrence
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 14, i64 n <= 22, i128 n <= 29,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_max = (idx_t)-1;   /* highest index for which cache[i] is set */

/* Helper to ensure the cache can hold at least indices 0..n */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Main function required by the build system */
seq_t a(idx_t n)
{
    /* Initialise the base values on first call */
    if (cache == NULL) {
        ensure_cache(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_max = 1;
    }

    ensure_cache(n);

    /* Compute any missing entries up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if ((i & 1ULL) == 0) {          /* even indices */
            cache[i] = (seq_t)1;
            continue;
        }

        /* For odd i use the defining recurrence */
        seq_t sum = 0;
        idx_t m = i - 1;                /* n-1 in the recurrence */
        for (idx_t k = 0; k <= m; ++k) {
            seq_t bin = binomial(m, k);
            seq_t term = bin * cache[k];
            term *= cache[m - k];
            if (k & 1ULL)
                sum -= term;
            else
                sum += term;
        }
        cache[i] = (seq_t)1 + sum;
    }
    cache_max = n;

    return cache[n];
}
