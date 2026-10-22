/*
 * A375289  Expansion of (1 - x + x^3)/((1 - x + x^3)^2 + 4*x^4).
 * https://oeis.org/A375289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 18, i32 n <= 34, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-2) - 2*a(n-3) - 2*a(n-4) - a(n-6)
   initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=0, a(4)=-5, a(5)=-14 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* dynamic cache of computed terms */
    static size_t cache_len = 0;      /* number of valid entries in cache */
    static size_t cache_cap = 0;      /* allocated capacity of cache */

    /* Return value if already computed */
    if (n < cache_len)
        return cache[n];

    /* Ensure the cache is large enough */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed)
            new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Constant initial values for indices 0..5 */
    static const seq_t init_vals[6] = {
        (seq_t)1,  (seq_t)1,  (seq_t)1,  (seq_t)0,
        (seq_t)-5, (seq_t)-14
    };

    /* Fill the base values if not already present */
    if (cache_len < 6) {
        size_t start = cache_len;
        size_t end   = 6;
        if (end > needed)
            end = needed;
        for (size_t i = start; i < end; ++i)
            cache[i] = init_vals[i];
        cache_len = end;
    }

    /* Compute missing terms using the recurrence */
    size_t i_start = cache_len > 6 ? cache_len : 6;
    for (size_t i = i_start; i < needed; ++i) {
        cache[i] = (seq_t)2 * cache[i-1]
                 - cache[i-2]
                 - (seq_t)2 * cache[i-3]
                 - (seq_t)2 * cache[i-4]
                 - cache[i-6];
    }

    cache_len = needed;
    return cache[n];
}
