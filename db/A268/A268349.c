/*
 * A268349  Expansion of (1 + x - x^2 - 6*x^3)/(1 - x - 2*x^2 - 3*x^3 -
 *          4*x^4).
 * https://oeis.org/A268349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2, a(2)=3, a(3)=4,
   a(n)=a(n-1)+2*a(n-2)+3*a(n-3)+4*a(n-4) for n>=4 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return initial values directly */
    if (n < 4) {
        static const seq_t init[4] = {1, 2, 3, 4};
        return init[n];
    }

    /* Static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (cache == NULL) {
        /* Allocate space for the first four terms */
        size_t cap = 4;
        if (n + 1 > cap) cap = n + 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 3;
        cache[3] = 4;
        cache_len = 4;
    }

    if (cache_len <= n) {
        if ((size_t)cache_len < n + 1) {
            size_t new_cap = n + 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val = cache[i - 1];
            val += ((seq_t)2 * cache[i - 2]);
            val += ((seq_t)3 * cache[i - 3]);
            val += ((seq_t)4 * cache[i - 4]);
            cache[i] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
