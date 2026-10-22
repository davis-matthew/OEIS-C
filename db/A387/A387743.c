/*
 * A387743  a(n) = (1/5) * Sum_{k=0..n-1} binomial(5*n,5*k+1).
 * https://oeis.org/A387743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A387743
 * Formula: a(n) = (1/5) * Σ_{k=0}^{n-1} binomial(5*n, 5*k+1)
 * Recurrence: a(n) = 21*a(n-1) + 353*a(n-2) - 32*a(n-3)  for n > 2
 * Initial values: a(0) = 0, a(1) = 1, a(2) = 44
 * Implementation uses memoised recurrence to compute quickly for any n.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoised values */
    static seq_t *cache = NULL;          /* pointer to array of seq_t values */
    static idx_t cache_size = 0;         /* allocated capacity (number of elements) */
    static idx_t last = 0;               /* highest index that has been computed */

    /* Initialise cache with first 3 terms on first call */
    if (!cache) {
        cache_size = 3;
        cache = (seq_t *)calloc((size_t)cache_size, sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 44;
        last = 2;
    }

    /* Direct return for indices 0..2 */
    if (n <= 2) {
        return cache[n];
    }

    /* Ensure cache has enough space for up to index n */
    if (n + 1 > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed; return 0 as a safe fallback (unlikely in tests) */
            return 0;
        }
        cache = tmp;
        cache_size = n + 1;
    }

    /* Compute missing values up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = 21 * cache[i - 1] + 353 * cache[i - 2] - 32 * cache[i - 3];
    }
    last = n;

    return cache[n];
}
