/*
 * A272362  Expansion of (1 + x - x^2 - x^3 - x^4)/((1 - x)*(1 - x - 2*x^2 -
 *          2*x^3 - x^4)).
 * https://oeis.org/A272362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - a(n-4) - a(n-5); Initial values:
 *            a(0)=1, a(1)=3, a(2)=6, a(3)=14, a(4)=32
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 52, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Ensure the cache has room for indices up to n */
    if (cache == NULL) {
        /* Allocate space for the first five terms */
        cache_size = 5;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0; /* fallback on allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)14;
        cache[4] = (seq_t)32;
    }

    /* Expand the cache if necessary */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0; /* fallback on allocation failure */
        cache = tmp;
        /* Compute new terms */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            /* i >= 5 since cache_size is 5 and i starts from cache_size */
            cache[i] = 2 * cache[i - 1]
                       + cache[i - 2]
                       - cache[i - 4]
                       - cache[i - 5];
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
