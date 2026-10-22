/*
 * A302588  a(n) = a(n-3) + 7*(n-2), a(0)=1, a(1)=2, a(2)=4.
 * https://oeis.org/A302588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-3) + 7*(n-2), a(0)=1, a(1)=2, a(2)=4
 * Range:     u8 n <= 15, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoized iterative computation.  Works for any build of seq_t. */
    static seq_t *cache   = NULL;
    static size_t capacity = 0;
    static size_t size     = 0;   /* number of values computed so far */

    /* Ensure we have at least n+1 entries in the cache. */
    size_t needed = (size_t)n + 1;
    if (needed > capacity) {
        size_t new_cap = capacity ? capacity * 2 : 4;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* memory allocation failed */
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute missing elements. */
    for (size_t i = size; i < needed; ++i) {
        if (i == 0)          cache[0] = (seq_t)1;
        else if (i == 1)     cache[1] = (seq_t)2;
        else if (i == 2)     cache[2] = (seq_t)4;
        else
            cache[i] = cache[i-3] + (seq_t)7 * ((seq_t)(i-2));
    }
    size = needed;
    return cache[n];
}
