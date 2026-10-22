/*
 * A350742  Sisyphus with squares: a(1) = 1; thereafter, if a(n-1) is even,
 *          a(n) = a(n-1)/2; otherwise a(n) = a(n-1) + k^2, where k^2 is the
 *          smallest square (starting from 1) that has not yet been added.
 * https://oeis.org/A350742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 252, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1)=1;
 *              for n>1:
 *               if a(n-1) even:  a(n)=a(n-1)/2;
 *               else:            a(n)=a(n-1)+k^2,
 *              where k^2 is the smallest unused square (1²,2²,3²,…). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* dynamic cache of computed values */
    static size_t   cap = 0;          /* capacity of cache */
    static idx_t    max_cached = 0;    /* highest index cached (1‑based) */
    static idx_t    next_k   = 1;      /* next square root to be used */

    /* n>=1 is required; behaviour undefined for n==0 */
    if (n < 1) return (seq_t)0;

    /* if we already have the requested value, return it immediately */
    if (n <= max_cached)
        return cache[n - 1];

    /* first call: allocate cache and set a(1)=1 */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)1;
        max_cached = 1;
        next_k = 1;
    }

    seq_t prev = cache[max_cached - 1];
    idx_t i;
    for (i = max_cached + 1; i <= n; ++i) {

        if ((prev & (seq_t)1) == 0) {                /* even */
            prev = prev / (seq_t)2;
        } else {                                     /* odd */
            seq_t sq = (seq_t)next_k * (seq_t)next_k;
            prev = prev + sq;
            ++next_k;
        }

        /* ensure enough room in the cache */
        if (i - 1 >= cap) {
            cap <<= 1;  /* double the capacity */
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cap);
            if (tmp == NULL) return prev;   /* allocation failure – return last computed */
            cache = tmp;
        }
        cache[i - 1] = prev;
    }

    max_cached = n;
    return prev;
}
