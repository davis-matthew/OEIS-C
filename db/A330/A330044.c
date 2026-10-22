/*
 * A330044  Expansion of e.g.f. exp(x) / (1 - x^3).
 * https://oeis.org/A330044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A330044: Expansion of exp(x)/(1-x^3)
   Recurrence:  a(0)=a(1)=a(2)=1
                a(n) = n*(n-1)*(n-2)*a(n-3) + 1   for n >= 3
   Base values and the recurrence are computed on demand and memoised. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache that grows only. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Grow the cache if n is beyond the current size. */
    if (n + 1u > cache_size) {
        idx_t old_size = cache_size;
        idx_t new_size = n + 1u;

        /* Allocate a new buffer and copy the existing values. */
        seq_t *tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!tmp)          /* allocation failed – return 0 as error code */
            return (seq_t)0;

        if (cache) {
            memcpy(tmp, cache, old_size * sizeof(seq_t));
            free(cache);
        }
        cache = tmp;
        cache_size = new_size;

        /* Initialise base cases 0,1,2 if they were not already present. */
        if (old_size < 3) {
            if (new_size > 0) cache[0] = (seq_t)1;
            if (new_size > 1) cache[1] = (seq_t)1;
            if (new_size > 2) cache[2] = (seq_t)1;
        }

        /* Compute any missing terms using the recurrence. */
        idx_t start = (old_size > 2) ? old_size : (idx_t)3;
        for (idx_t i = start; i <= n; ++i) {
            cache[i] =
                (seq_t)i * ((seq_t)i - (seq_t)1) *
                ((seq_t)i - (seq_t)2) * cache[i - 3] + (seq_t)1;
        }
    }

    return cache[n];
}
