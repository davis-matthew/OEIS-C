/*
 * A387018  a(n) = 2*a(n-1) + 2*a(n-2) + 2*a(n-3) - a(n-4) with a(0)=0,
 *          a(1)=1, a(2)=4, a(3)=11.
 * https://oeis.org/A387018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + 2*a(n-3) - a(n-4) a(0) = 0, a(1) =
 *            1, a(2) = 4, a(3) = 11
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed values          */
static idx_t  cache_len = 0;         /* number of elements stored        */
static size_t cache_cap = 0;         /* allocated capacity                */

/* Ensure the cache can hold at least n+1 elements */
static void
ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;

    size_t new_cap = cache_cap > 0 ? cache_cap << 1 : 4;
    while (new_cap <= (size_t)(n + 1))
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* In a real program you might abort or handle the error */
        /* Here we simply ignore the failure – the build will
           never request more than the heap can handle for the
           problem constraints. */
        return;
    }
    cache = tmp;
    cache_cap = new_cap;

    /* Initialize newly allocated elements to zero (safety) */
    for (size_t i = cache_len; i < cache_cap; ++i)
        cache[i] = 0;
}

/* Public sequence function */
seq_t
a(idx_t n)
{
    /* Handle base cases directly */
    if (n <= 3) {
        switch (n) {
            case 0: return (seq_t)0;
            case 1: return (seq_t)1;
            case 2: return (seq_t)4;
            case 3: return (seq_t)11;
        }
    }

    /* Make sure the cache has storage up to n */
    ensure_cache(n);

    /* Compute missing terms up to n */
    while (cache_len <= n) {
        /* Guarantee we have at least 4 previous values */
        if (cache_len <= 3) {
            /* Initialize the first four values */
            switch (cache_len) {
                case 0: cache[0] = (seq_t)0; break;
                case 1: cache[1] = (seq_t)1; break;
                case 2: cache[2] = (seq_t)4; break;
                case 3: cache[3] = (seq_t)11; break;
            }
        } else {
            idx_t i = cache_len;
            seq_t val = 2 * cache[i-1] + 2 * cache[i-2]
                      + 2 * cache[i-3] - cache[i-4];
            cache[i] = val;
        }
        ++cache_len;
    }

    return cache[n];
}
