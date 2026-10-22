/*
 * A317762  Number of nX6 0..1 arrays with every element unequal to 0, 1 or 2
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A317762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3) - a(n-4) + a(n-7) for n > 10
 *            Initial values: a(1)=32, a(2)=89, a(3)=100, a(4)=127, a(5)=177,
 *            a(6)=266, a(7)=425, a(8)=709, a(9)=1217, a(10)=2126. This
 *            function memoises the values up to the largest n requested
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 34, u64 n <= 73, u128 n <= 149,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Hard‑coded initial values (required for the recurrence). */
    static seq_t init[10] = {32, 89, 100, 127, 177, 266, 425, 709, 1217, 2126};

    /* Static cache for memoised values. */
    static seq_t *cache = NULL;     /* array of a(k) values, 1‑based index => 0‑based array */
    static size_t cache_len = 0;    /* number of values stored in cache */
    static idx_t max_computed = 0;  /* largest n for which a(n) is stored */

    /* Handle obvious out‑of‑range requests.  For n <= 0 we simply return 0. */
    if (n <= 0) {
        return (seq_t)0;
    }

    /* Ensure cache is initialised. */
    if (cache_len == 0) {
        /* Allocate space for the first 10 terms. */
        cache = (seq_t *)malloc(sizeof(seq_t) * 10);
        if (!cache) {
            /* Allocation failure – return 0 (undefined behaviour otherwise). */
            return (seq_t)0;
        }
        for (idx_t i = 0; i < 10; ++i) {
            cache[i] = init[i];
        }
        cache_len = 10;
        max_computed = 10;
    }

    /* If the requested n is within the cache, return immediately. */
    if (n <= (idx_t)cache_len) {
        return cache[n - 1];
    }

    /* Grow the cache to accommodate up to index n. */
    size_t new_len = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (!tmp) {
        /* Reallocation failed – return 0. */
        return (seq_t)0;
    }
    cache = tmp;
    cache_len = new_len;

    /* Compute missing terms up to n using the recurrence relation. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* Indices are 1‑based for a(k).  Convert to 0‑based for the array. */
        seq_t term =
            2 * cache[i - 2] +          /* 2*a(i-1) */
            cache[i - 3] +              /* + a(i-2) */
            (cache[i - 4] * (seq_t)(-2))/* - 2*a(i-3) */
            + (cache[i - 5] * (seq_t)(-1))/* - a(i-4) */
            + cache[i - 8];             /* + a(i-7) */
        cache[i - 1] = term;
    }

    max_computed = n;
    return cache[n - 1];
}
