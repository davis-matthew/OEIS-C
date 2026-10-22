/*
 * A355163  a(n) = exp(-1) * Sum_{k>=0} (4*k + 3)^n / k!.
 * https://oeis.org/A355163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * a(n-1) + Σ_{k=1..n} binomial(n-1,k-1) * 4^k * a(n-k),
 *            a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;      /* cache[0..k] contains a(0)…a(k) */
static size_t cache_size = 0;   /* size of allocated cache */

/* Ensure that cache contains at least n+1 elements (0..n) */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {           /* memory allocation failed */
            exit(1);
        }
        /* Initialise newly allocated entries to zero */
        for (idx_t i = cache_size; i < new_size; ++i) {
            tmp[i] = 0;
        }
        cache = tmp;
        cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    /* Handle the base case specially */
    if (n == 0) {
        ensure_cache(0);
        if (cache[0] == 0)
            cache[0] = 1;
        return cache[0];
    }

    ensure_cache(n);

    /* If the value was already computed, return it directly */
    if (cache[n] != 0)
        return cache[n];

    /* Compute all missing terms up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != 0)          /* term already known */
            continue;

        seq_t val = 3 * cache[i - 1];   /* 3 * a(i-1)   */
        seq_t pow4 = 4;                 /* 4^1 initially */

        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = binomial(i - 1, k - 1);  /* binomial coefficient */
            term *= pow4;                         /* *4^k            */
            term *= cache[i - k];                 /* *a(i-k)         */
            val += term;
            pow4 *= 4;                            /* ready for next k */
        }

        cache[i] = val;
    }

    return cache[n];
}
