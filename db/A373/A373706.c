/*
 * A373706  Expansion of 1/(1 - x * (1 + x^4)^2).
 * https://oeis.org/A373706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A373706: Expansion of 1/(1 - x * (1 + x^4)^2)
   Recurrence: a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=1,
                a(5)=3, a(6)=5, a(7)=7, a(8)=9,
                a(n) = a(n-1) + 2*a(n-5) + a(n-9)   for n > 8 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t   cache_cap = 0;   /* allocated array size          */
    static idx_t   cache_max = 0;   /* largest index already computed */

    /* ---- initialise cache with first 9 values ---- */
    if (cache_cap == 0) {
        cache_cap = 9;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure (unlikely) */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)3;
        cache[6] = (seq_t)5;
        cache[7] = (seq_t)7;
        cache[8] = (seq_t)9;

        cache_max = 8;
    }

    /* ---- return already known value ---- */
    if (n <= cache_max)
        return cache[n];

    /* ---- grow cache if necessary ---- */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n)
            new_cap <<= 1;                     /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failure      */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* ---- compute terms from cache_max+1 up to n ---- */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* recurrence: a(i) = a(i-1) + 2*a(i-5) + a(i-9)   for i > 8 */
        cache[i] = cache[i - 1]
                 + (seq_t)2 * cache[i - 5]
                 + cache[i - 9];
    }

    cache_max = n;
    return cache[n];
}
