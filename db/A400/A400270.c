/*
 * A400270  G.f. A(x) satisfies A(x) = (1 + x) * A(x^3 * (1 + x)^5), with A(0)
 *          = 1.
 * https://oeis.org/A400270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{k=0..⌊n/3⌋} binomial(5*k+1, n-3*k) * a(k) with a(0)
 *            = 1
 * Range:     u8 n <= 11, u16 n <= 15, u32 n <= 29, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;    /* cache[i] holds a(i) for 0 <= i < cache_len */
static size_t cache_len = 0;   /* number of entries already computed */
static size_t cache_cap = 0;   /* allocated capacity of cache */

/* Ensure that cache contains a(0)…a(n).  Compute any missing values. */
static void ensure_cache(idx_t n)
{
    /* Grow the cache if necessary */
    if (cache == NULL) {
        cache_cap = n + 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    } else if (cache_cap <= n) {
        cache_cap = n + 1;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    }

    /* Compute all new terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1; /* initial value */
        } else {
            seq_t sum = 0;
            idx_t maxk = i / 3;
            for (idx_t k = 0; k <= maxk; ++k) {
                idx_t b = i - 3 * k;          /* n - 3*k */
                seq_t bin = binomial(5 * k + 1, b);
                if (bin != 0)                 /* skip zero binomials */
                    sum += bin * cache[k];
            }
            cache[i] = sum;
        }
    }
    cache_len = n + 1;   /* new last valid index */
}

/* Public function: returns a(n) for any n >= 0, in any call order. */
seq_t a(idx_t n)
{
    if (n < cache_len)
        return cache[n];
    ensure_cache(n);
    return cache[n];
}
