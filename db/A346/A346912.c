/*
 * A346912  a(0) = 1; a(n) = a(n-1) + a(floor(n/2)) + 1.
 * https://oeis.org/A346912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=a(n-1)+a(floor(n/2))+1 for n>=1
 * Range:     u8 n <= 13, u16 n <= 66, u32 n <= 506, u64 n <= 7336,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;     /* number of elements allocated          */
    static idx_t last_computed = 0; /* highest index that has a value computed */

    /* Initialise on first call */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;
        cache[0] = (seq_t)1;
        last_computed = 0;
    }

    /* If we need more entries, grow the allocation */
    if (n > last_computed && n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        /* Grow by doubling until we exceed n+1, but never overflow */
        while (new_cap <= (size_t)n) {
            if (new_cap > SIZE_MAX / 2) {        /* prevent overflow */
                new_cap = (size_t)n + 1;
                break;
            }
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i / 2] + (seq_t)1;
    }
    last_computed = n;

    return cache[n];
}
