/*
 * A281241  Solutions x to the negative Pell equation y^2 = 72*x^2 - 1331712
 *          with x,y >= 0.
 * https://oeis.org/A281241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 31, u64 n <= 69, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A281241:  Solutions x to the negative Pell equation
 * y^2 = 72*x^2 - 1331712  (x,y >= 0)
 * Recurrence (offset 1):  a(n) = 6*a(n-3) - a(n-6)  for n>6
 * Initial values:
 *   a(1)=136, a(2)=152, a(3)=264, a(4)=408, a(5)=648, a(6)=1432
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;          /* pointer to cached array */
    static idx_t cache_len = 0;          /* number of terms cached    */
    static const seq_t init[6] = {
        136, 152, 264, 408, 648, 1432
    };

    if (n == 0) return 0;                /* offset starts at 1 */

    /* initialise cache with first six terms on first call */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 6);
        if (!cache) return 0;            /* allocation failure unlikely */
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = init[i];
        cache_len = 6;
    }

    /* if n is within cached range, return it */
    if (n <= cache_len)
        return cache[n - 1];

    /* extend cache up to n */
    size_t new_bytes = (size_t)n * sizeof(seq_t);
    seq_t *tmp = (seq_t *)realloc(cache, new_bytes);
    if (!tmp) {
        /* on realloc failure keep old cache */
        return 0;
    }
    cache = tmp;

    for (idx_t i = cache_len; i < n; ++i) {
        /* recurrence: a(i+1) = 6*a(i-2) - a(i-5)
         * (zero‑based: cache[i] = 6*cache[i-3] - cache[i-6]) */
        cache[i] = (seq_t)6 * cache[i - 3] - cache[i - 6];
    }

    cache_len = n;
    return cache[n - 1];
}
