/*
 * A284314  Expansion of Product_{k>=0} (1 - x^(5*k+1)) in powers of x.
 * https://oeis.org/A284314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥0} (1 - x^(5*k+1)) (OEIS
 *            A284314)
 * Range:     i8 n <= 143, i16 n <= 396, i32 n <= 1226, i64 n <= 4121,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoised coefficients */
static seq_t *cache   = NULL;   /* cache[0..cache_cap-1] */
static idx_t cache_cap = 0;    /* number of elements allocated */

/* Recompute all coefficients up to and including n, store in cache */
static void recompute(idx_t n)
{
    /* Allocate new array */
    size_t sz = (size_t)(n + 1U);
    seq_t *new_cache = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!new_cache) {
        /* In a rare OOM situation abort – the harness will not try
           to recover from this. */
        exit(1);
    }

    /* Initialise all entries to 0, set a[0] = 1 */
    for (idx_t i = 0; i <= n; ++i)
        new_cache[i] = 0;
    new_cache[0] = 1;

    /* Multiply in each factor (1 - x^e) for e = 1,6,11,… ≤ n */
    for (idx_t e = 1; e <= n; e += 5) {
        for (idx_t i = n; i >= e; --i) {
            new_cache[i] -= new_cache[i - e];
        }
    }

    /* Replace old cache */
    free(cache);
    cache = new_cache;
    cache_cap = n + 1U;
}

/* Public entry point required by the test harness */
seq_t a(idx_t n)
{
    /* Ensure the cache contains terms up to n */
    if (cache_cap <= n)
        recompute(n);
    return cache[n];
}
