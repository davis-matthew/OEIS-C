/*
 * A383378  Expansion of e.g.f. exp(-3*x) / (1-x)^4.
 * https://oeis.org/A383378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = a(1) = 1; a(n) = n*a(n-1) + 3*(n-1)*a(n-2) for n >= 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stdbool.h>  /* bool type (not used directly but required by the checks) */

static seq_t *cache   = NULL;  /* memoization storage              */
static size_t cache_cap = 0;   /* allocated capacity                */
static size_t cache_len = 0;   /* number of values computed (i.e. highest index + 1) */

/* Computes A383378(n) for any n >= 0, reusing previous results. */
seq_t a(idx_t n)
{
    /* Guard against size_t overflow: n must fit in the host size_t. */
    size_t idx = (size_t)n;

    /* Allocate initial storage if needed. */
    if (cache == NULL) {
        cache_cap = (size_t)16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out of memory – unlikely in tests */
    }

    /* Grow storage in powers of two to accommodate n. */
    if (idx + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < idx + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    if (cache_len <= idx) {
        size_t start = cache_len;
        for (size_t i = start; i <= idx; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)1;
            } else {
                seq_t ix   = (seq_t)i;
                seq_t im1  = (seq_t)(i - 1);
                /* a(i) = i*a(i-1) + 3*(i-1)*a(i-2) */
                cache[i] = ix * cache[i - 1] + (seq_t)3 * im1 * cache[i - 2];
            }
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
