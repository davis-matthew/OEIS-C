/*
 * A370074  Expansion of (1 - 2*x) * (1 - 4*x + 2*x^2) / (1 - 9*x + 28*x^2 -
 *          35*x^3 + 15*x^4 - x^5).
 * https://oeis.org/A370074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 28*a(n-2) + 35*a(n-3) - 15*a(n-4) + a(n-5) for
 *            n >= 5, with initial values: a(0) = 1, a(1) = 3, a(2) = 9, a(3)
 *            = 28, a(4) = 90
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;  /* array of computed terms */
    static size_t cache_sz = 0;    /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* initialise cache with first five terms on first call */
    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)28;
        cache[4] = (seq_t)90;
        cache_sz = 5;
    }

    /* if requested index already computed, return it */
    if (n < cache_sz)
        return cache[n];

    /* enlarge cache if necessary */
    size_t needed = (size_t)n + 1;           /* need up to index n */
    if (needed > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        cache    = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (size_t i = cache_sz; i <= n; ++i) {
        seq_t term =
            (seq_t)9 * cache[i-1]
          - (seq_t)28 * cache[i-2]
          + (seq_t)35 * cache[i-3]
          - (seq_t)15 * cache[i-4]
          +        cache[i-5];
        cache[i] = term;
    }
    cache_sz = needed;

    return cache[n];
}
