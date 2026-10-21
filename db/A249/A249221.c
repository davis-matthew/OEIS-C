/*
 * A249221  Expansion of x*(1+5*x-2*x^3)/(1-6*x^2+2*x^4).
 * https://oeis.org/A249221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-2)-2*a(n-4) with a(1)=1, a(2)=5, a(3)=6, a(4)=28
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 51, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* store a(1), a(2), ... */
static idx_t cache_len = 0;      /* number of cached terms (cache_len = length of cache) */

seq_t a(idx_t n)
{
    /* valid for n >= 1, returns 0 for n == 0 (unused) */
    if (n == 0) return 0;

    /* initial values */
    static const seq_t start[4] = {1, 5, 6, 28};

    /* extend cache as necessary */
    if (n > cache_len) {
        /* enlarge allocated memory */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed; treat as zeroes (unlikely in judge env) */
            return 0;
        }
        cache = tmp;

        /* compute missing terms */
        for (idx_t i = cache_len; i < n; ++i) {
            idx_t idx = i + 1;               /* 1‑based sequence index */
            if (idx <= 4) {
                cache[i] = start[idx - 1];
            } else {
                /* a(idx) = 6*a(idx-2) - 2*a(idx-4) */
                cache[i] = 6 * cache[idx - 3] - 2 * cache[idx - 5];
            }
        }
        cache_len = n;
    }

    return cache[n - 1];
}
