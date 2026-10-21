/*
 * A290662  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 899", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=5, a(3)=15; a(n)=a(n-1)+4*a(n-2)-4*a(n-3)
 *            for n>3
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = (seq_t *)NULL;
    static idx_t cached_last = 0;   /* last index that is filled in cache */
    static idx_t cache_size = 0;   /* allocated size of cache (indices 0 .. cache_size-1) */

    /* Initialize cache on first call */
    if (cache == (seq_t *)NULL) {
        idx_t init_cap = 4;
        cache = (seq_t *)malloc((size_t)init_cap * sizeof(seq_t));
        cache_size = init_cap;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)15;
        cached_last = 3;
    }

    /* Grow cache if needed */
    if (n > cached_last) {
        idx_t new_cap = n + 1;
        if (new_cap > cache_size) {
            seq_t *tmp = (seq_t *)realloc(cache,
                                          (size_t)new_cap * sizeof(seq_t));
            cache = tmp;
            cache_size = new_cap;
        }

        /* Fill missing values using the recurrence */
        for (idx_t i = cached_last + 1; i <= n; ++i) {
            seq_t t = cache[i - 1];
            t += (seq_t)4 * cache[i - 2];
            t -= (seq_t)4 * cache[i - 3];
            cache[i] = t;
        }
        cached_last = n;
    }

    return cache[n];
}
