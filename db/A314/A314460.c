/*
 * A314460  Coordination sequence Gal.4.110.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-6) - a(n-8) for n > 16 Initial values:
 *            a(0)=1, a(1)=5, a(2)=8, a(3)=13, a(4)=20, a(5)=23, a(6)=27,
 *            a(7)=36, a(8)=37, a(9)=45, a(10)=50, a(11)=55, a(12)=59,
 *            a(13)=68, a(14)=67, a(15)=77, a(16)=78
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* the 17 initial terms */
    static const seq_t init[17] = {
        (seq_t)1,  (seq_t)5,  (seq_t)8,  (seq_t)13,
        (seq_t)20, (seq_t)23, (seq_t)27, (seq_t)36,
        (seq_t)37, (seq_t)45, (seq_t)50, (seq_t)55,
        (seq_t)59, (seq_t)68, (seq_t)67, (seq_t)77,
        (seq_t)78
    };

    /* initialize the cache up to index 16 */
    if (!cache) {
        cache_cap = 17;
#ifdef __cplusplus
        cache = reinterpret_cast<seq_t*>(malloc(cache_cap * sizeof(seq_t)));
#else
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
#endif
        if (!cache) exit(1); /* abort on allocation failure */
        for (size_t i = 0; i < cache_cap; ++i)
            cache[i] = init[i];
    }

    /* if n is already cached, just return it */
    if ((size_t)n < cache_cap)
        return cache[(size_t)n];

    /* extend cache to cover n */
    size_t new_cap = (size_t)n + 1;
#ifdef __cplusplus
    seq_t *tmp = reinterpret_cast<seq_t*>(realloc(cache, new_cap * sizeof(seq_t)));
#else
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
    if (!tmp) exit(1);
    cache = tmp;

    /* compute missing values */
    for (size_t i = cache_cap; i <= (size_t)n; ++i) {
        if (i < 17) { /* should not happen, but keep for safety */
            cache[i] = init[i];
        } else {
            /* recurrence: a(n) = a(n-2) + a(n-6) - a(n-8) */
            cache[i] = cache[i-2] + cache[i-6] - cache[i-8];
        }
    }

    cache_cap = new_cap;
    return cache[(size_t)n];
}
