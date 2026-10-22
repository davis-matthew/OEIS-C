/*
 * A367713  G.f. A(x) satisfies A(x) = 1 / ((1 - x) * (1 - x * (1 + x) *
 *          A(x^2))).
 * https://oeis.org/A367713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + \sum_{k=0}^{n-1} a(\lfloor k/2 \rfloor) * a(n-1-k)
 *            with a(0) = 1
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache storage for memoization */
static seq_t *cache     = NULL;   /* array of previously computed values */
static size_t cache_cap = 0;      /* allocated length of cache */
static size_t cache_sz  = 0;      /* number of entries currently stored */

/* Ensure the cache contains values up to and including a[n] */
static void _ensure_cache(idx_t n)
{
    /* Expand cache if necessary */
    if (cache_cap == 0) {
        cache_cap = (size_t)(n + 1);
        if (cache_cap == 0) cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)(n + 1))
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialise the first value if cache is empty */
    if (cache_sz == 0) {
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        cache_sz = 1;
    }

    /* Compute terms from cache_sz up to n (inclusive) */
    for (idx_t i = (idx_t)cache_sz; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t j = 0; j < i; ++j) {
            idx_t f = j >> 1;              /* floor(j/2) */
            idx_t r = (i - 1) - j;         /* n-1-k */
            sum += cache[f] * cache[r];
        }
        cache[i] = sum + (seq_t)1;         /* add the +1 term */
    }
    cache_sz = (size_t)(n + 1);
}

/* External API: compute a(n) */
seq_t a(idx_t n)
{
    _ensure_cache(n);
    return cache[(size_t)n];
}
