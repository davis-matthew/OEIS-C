/*
 * A299286  Partial sums of A299285.
 * https://oeis.org/A299286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-4) - 3*a(n-5) +
 *            3*a(n-6) - a(n-7) Initial values: a(0)=1, a(1)=11, a(2)=44,
 *            a(3)=117, a(4)=245, a(5)=444, a(6)=729, a(7)=1117
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* size_t */
#include "../utils/grow_seq.h"   /* grow_seq */

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t        *cache = NULL;
    static size_t        cache_cap = 0;
    static idx_t         cache_size = 0;

    /* static copy of the first 8 terms, cast to seq_t to keep
       correct modulo behaviour for any seq_t width */
    static const seq_t init[8] = {
        (seq_t)1,   (seq_t)11,  (seq_t)44,
        (seq_t)117, (seq_t)245, (seq_t)444,
        (seq_t)729, (seq_t)1117
    };

    /* extend cache storage if needed */
    if ((size_t)n + 1 > cache_cap)
        grow_seq(&cache, &cache_cap, (size_t)n + 1);

    /* initialise first eight terms once, respecting cache limits */
    if (cache_size == 0) {
        size_t init_len = cache_cap < 8 ? cache_cap : 8;
        for (size_t i = 0; i < init_len; ++i)
            cache[i] = init[i];
        cache_size = (idx_t)init_len;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 8) continue;                       /* already set */
        cache[i] =  3*cache[i-1] - 3*cache[i-2] + cache[i-3]
                  + cache[i-4] - 3*cache[i-5] + 3*cache[i-6]
                  - cache[i-7];
    }
    cache_size = n + 1;

    return cache[n];
}
