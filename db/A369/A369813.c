/*
 * A369813  Expansion of 1/(1 - x^2 - x^7).
 * https://oeis.org/A369813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-7), a(0)=1, a(1)=0, a(2)=1
 * Range:     u8 n <= 38, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoized computation of A369813: number of compositions of n into 2s and 7s. */
seq_t a(idx_t n)
{
    /* Direct base cases for n < 2 */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* Static cache to store already computed values */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;

    /* Ensure the cache can hold up to n */
    if (n + 1 > max_cached) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* allocation failed – return 0 as a safe fallback */

        /* Copy existing cached values, if any */
        if (cache) {
            for (idx_t i = 0; i < max_cached; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        max_cached = new_size;

        /* Initialise known terms */
        if (max_cached > 0) cache[0] = (seq_t)1;
        if (max_cached > 1) cache[1] = (seq_t)0;
        if (max_cached > 2) cache[2] = (seq_t)1;
    }

    /* Compute any missing terms up to n using the recurrence */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t val = 0;
        if (i >= 2) val += cache[i - 2];
        if (i >= 7) val += cache[i - 7];
        cache[i] = val;
    }

    return cache[n];
}
