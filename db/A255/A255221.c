/*
 * A255221  Number of (n+2) X (1+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum 0 and no antidiagonal sum 0 and no row sum 2 and no
 *          column sum 2.
 * https://oeis.org/A255221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 23, u32 n <= 52, u64 n <= 110, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4) for n > 5
   Initial values: a(1)=23, a(2)=22, a(3)=31, a(4)=43, a(5)=61 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;   /* cache[1] holds a(1) */
static idx_t cache_max = 0;   /* maximum n for which cache[n] is valid */

/* Helper to ensure cache is large enough and compute missing values */
static void
ensure_cache(idx_t n)
{
    if (n <= cache_max)
        return;

    /* allocate or grow cache */
    if (cache == NULL) {
        cache = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        cache[0] = 0;          /* unused */
    } else if (n > cache_max) {
        cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    }

    /* compute missing terms */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i == 1)
            cache[i] = (seq_t)23;
        else if (i == 2)
            cache[i] = (seq_t)22;
        else if (i == 3)
            cache[i] = (seq_t)31;
        else if (i == 4)
            cache[i] = (seq_t)43;
        else if (i == 5)
            cache[i] = (seq_t)61;
        else {
            /* a(i) = 2*a(i-1) - a(i-2) + a(i-3) - a(i-4) */
            cache[i] =
                cache[i-1] * (seq_t)2
                - cache[i-2]
                + cache[i-3]
                - cache[i-4];
        }
    }
    cache_max = n;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for 0, return 0 */
        return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
