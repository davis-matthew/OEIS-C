/*
 * A280756  Expansion of (1 + 2*x)/(1 - x - 4*x^2 + x^4).
 * https://oeis.org/A280756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Expansion of (1+2*x)/(1 - x - 4*x^2 + x^4). Recurrence: a(n) =
 *            a(n-1) + 4*a(n-2) - a(n-4) for n >= 4 with a(0)=1, a(1)=3,
 *            a(2)=7, a(3)=19
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External integer types defined by the build environment */
extern seq_t a(idx_t n);

/* Static cache for memoisation */
static seq_t *cache = NULL;   /* array of computed terms */
static idx_t cache_len = 0;   /* number of terms currently stored */
static idx_t cache_cap = 0;   /* allocated capacity of cache */

/* Ensure that cache can hold values up to index n and compute missing terms. */
static void ensure_cache(idx_t n)
{
    /* Expand capacity if needed */
    if (n + 1 > cache_cap) {
        idx_t new_cap = (cache_cap > 0) ? cache_cap : 4;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base cases on first entry */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)19;
        cache_len = 4;
    }

    /* Compute missing terms using recurrence */
    if (n + 1 > cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val = cache[i-1] + 4 * cache[i-2] - cache[i-4];
            cache[i] = val;
        }
        cache_len = n + 1;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
