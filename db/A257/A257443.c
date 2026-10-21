/*
 * A257443  Number of (n+2) X (4+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum 0 and no antidiagonal sum 3 and no row sum 1 and no
 *          column sum 1.
 * https://oeis.org/A257443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4) (for n > 6) Initial
 *            values: a(1) = 49, a(2) = 51, a(3) = 59, a(4) = 71, a(5) = 89,
 *            a(6) = 116 The sequence is cached so that any call to a(n) (with
 *            n ≥ 1) returns the correct value, regardless of call order
 * Range:     u8 n <= 8, u16 n <= 23, u32 n <= 52, u64 n <= 110, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache: cache[i] holds a(i) for i ≥ 1.  Index 0 is unused. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Helper to grow the cache up to index n and compute missing terms. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;

    /* Allocate space for indices 1..n (index 0 unused). */
    size_t new_len = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) abort();          /* memory allocation failed */
    cache = new_cache;

    /* Initial values for the first six terms. */
    static const seq_t init[6] = {49, 51, 59, 71, 89, 116};

    /* Compute any missing terms up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        if (i <= 6)
            cache[i] = init[i - 1];
        else
        {
            seq_t t = 2 * cache[i - 1];
            t -= cache[i - 2];
            t += cache[i - 3];
            t -= cache[i - 4];
            cache[i] = t;
        }
    }

    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 – 0 is undefined */
    ensure_cache(n);
    return cache[n];
}
