/*
 * A332495  a(n-2) = a(n-6) + 5*(1+2*n) with a(0)=0, a(1)=2, a(2)=7, a(3)=15
 *          for n>=4.
 * https://oeis.org/A332495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 4*a(n-3) - 3*a(n-4) + a(n-5) for n
 *            >= 5, with a(0)=0, a(1)=2, a(2)=7, a(3)=15, a(4)=25
 * Range:     u8 n <= 13, u16 n <= 228, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache if not yet done */
    if (cache_len == 0) {
        /* reserve space for at least first 5 terms */
        cache_len = 5;
        cache = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
        if (!cache) abort();  /* allocation failure */
        /* base values */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)15;
        cache[4] = (seq_t)25;
    }

    /* if requested value already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* grow cache to accommodate n */
    if (n + 1 > cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        /* compute new terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* recurrence: a(i) = 3*a(i-1) - 4*a(i-2) + 4*a(i-3) - 3*a(i-4) + a(i-5) */
            seq_t t1 = ((seq_t)3) * cache[i - 1];
            seq_t t2 = ((seq_t)4) * cache[i - 2];
            seq_t t3 = ((seq_t)4) * cache[i - 3];
            seq_t t4 = ((seq_t)3) * cache[i - 4];
            cache[i] = t1 - t2 + t3 - t4 + cache[i - 5];
        }
        cache_len = (idx_t)new_len;
    }

    return cache[n];
}
