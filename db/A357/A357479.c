/*
 * A357479  a(n) = (n!/6) * Sum_{k=0..n-3} 1/k!.
 * https://oeis.org/A357479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*a(n-1) + binomial(n,3), a(0)=0, a(1)=0, a(2)=0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_max = 2;

    /* first call: allocate an initial cache */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        if (cache_cap < 3)
            cache_cap = 3;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
    }

    /* extend cache if needed */
    if (n > cache_max) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap > cache_cap) {
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t si   = (seq_t)i;
            seq_t si1  = (seq_t)(i - 1);
            seq_t si2  = (seq_t)(i - 2);
            seq_t bin  = si * si1 * si2 / 6;        /* binomial(i,3) */
            cache[i]   = si * cache[i - 1] + bin;   /* recurrence */
        }
        cache_max = n;
    }

    return cache[n];
}
