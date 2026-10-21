/*
 * A252585  Numbers n such that the sum of the pentagonal numbers P(n) and
 *          P(n+1) is equal to the heptagonal number H(m) for some m.
 * https://oeis.org/A252585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 482*a(n-2) – 482*a(n-3) – a(n-4) + a(n-5) * with
 *            initial values * a(1) = 3, a(2) = 234, a(3) = 1617, a(4) =
 *            112948, a(5) = 779551
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 1-indexed cache to memoise computed terms.
       cache[0] is unused. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size (>= n+1) */
    static idx_t  cache_size = 0;  /* highest index stored in cache */

    if (n == 0)          /* behaviour for indices below the offset is irrelevant */
        return 0;

    /* Grow the cache to hold at least indices 1 … n */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* realloc preserves contents; cast needed for C++. */
        if (!tmp)                /* allocation failure – just return 0 */
            return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to index n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        switch (i) {
            case 1:  cache[1] = (seq_t)3; break;
            case 2:  cache[2] = (seq_t)234; break;
            case 3:  cache[3] = (seq_t)1617; break;
            case 4:  cache[4] = (seq_t)112948; break;
            case 5:  cache[5] = (seq_t)779551; break;
            default:
                cache[i] = cache[i-1]
                         + (seq_t)482 * cache[i-2]
                         - (seq_t)482 * cache[i-3]
                         - cache[i-4]
                         + cache[i-5];
                break;
        }
    }

    cache_size = n;
    return cache[n];
}
