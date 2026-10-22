/*
 * A306380  Squares of the form 5*k^2 + 5.
 * https://oeis.org/A306380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 323*a(n-1) - 323*a(n-2) + a(n-3) (n>3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1)=25, a(2)=7225, a(3)=2325625 */

#include <stdlib.h>

/* seq_t and idx_t are provided by the build environment. */
seq_t a(idx_t n)
{
    /* Thread‑safe static state.  The array is 1‑based; index 0 is unused. */
    static seq_t *cache = NULL;   /* Cached values */
    static size_t cache_size = 0; /* Allocated size (array length) */
    static idx_t  cached_to = 0;  /* Highest index that has been computed */

    if (n == 0) return (seq_t)0;  /* The sequence is defined only for n>=1 */

    /* Ensure enough storage. */
    if (n + 1 > cache_size) {
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* Allocation failed – return 0 (unlikely). */
        cache   = tmp;
        cache_size = new_size;
    }

    /* Compute any missing entries up to n. */
    if (n > cached_to) {
        for (idx_t i = cached_to + 1; i <= n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)25;
            } else if (i == 2) {
                cache[i] = (seq_t)7225;
            } else if (i == 3) {
                cache[i] = (seq_t)2325625;
            } else {
                /* Recurrence: a(n) = 323*(a(n-1) - a(n-2)) + a(n-3) */
                cache[i] = (seq_t)323 * (cache[i-1] - cache[i-2]) + cache[i-3];
            }
        }
        cached_to = n;
    }

    return cache[n];
}
