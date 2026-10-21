/*
 * A289919  p-INVERT of (1,0,0,1,0,0,1,0,0,...), where p(S) = (1 - S)^2.
 * https://oeis.org/A289919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - 2*a(n-4) - a(n-6), *
 *            a(0)=2, a(1)=3, a(2)=4, a(3)=7, a(4)=12, a(5)=19, a(6)=30
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 51, u64 n <= 107,
 *            u128 n <= 221, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* current allocated size */

    /* first call: allocate space and set the first seven terms */
    if (cache == NULL) {
        idx_t init_cap = (n < 7) ? 7 : n + 1;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */

        cache[0] = (seq_t)2;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)7;
        cache[4] = (seq_t)12;
        cache[5] = (seq_t)19;
        cache[6] = (seq_t)30;
        cap = init_cap;
    }

    /* already computed? */
    if (n < cap) {
        return cache[n];
    }

    /* grow cache to accommodate index n */
    idx_t old_cap = cap;
    idx_t new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) return (seq_t)0;   /* allocation failure */
    cache = tmp;
    cap   = new_cap;

    /* compute missing terms using the recurrence */
    for (idx_t i = old_cap; i <= n; ++i) {
        if (i < 7) {
            /* base values already stored */
            continue;
        }
        cache[i] =  ( (seq_t)2 * cache[i-1] )
                 - cache[i-2]
                 + ( (seq_t)2 * cache[i-3] )
                 - ( (seq_t)2 * cache[i-4] )
                 - cache[i-6];
    }

    return cache[n];
}
