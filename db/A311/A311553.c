/*
 * A311553  Coordination sequence Gal.6.133.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12 Initial values:
 *            a(0)=1, a(1)=4, a(2)=8, a(3)=12, a(4)=17, a(5)=23, a(6)=29,
 *            a(7)=35, a(8)=40, a(9)=44, a(10)=48, a(11)=52, a(12)=56
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;      /* array of computed terms   */
    static size_t cap    = 0;           /* current capacity of cache */
    static size_t sz     = 0;           /* number of terms currently stored */

    /* first 13 terms (a(0) .. a(12)) */
    static const seq_t init[13] = {
        1, 4, 8, 12, 17, 23, 29, 35, 40, 44, 48, 52, 56
    };

    /* If the cache is empty, allocate and fill the initial terms. */
    if (cache == NULL) {
        /* Allocate a capacity large enough for at least the first 13 terms. */
        cap = 13;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();           /* OOM – abort is safe here          */
        for (size_t i = 0; i < 13; ++i) cache[i] = init[i];
        sz = 13;
    }

    /* Grow the cache if we need to access terms beyond its current capacity. */
    if (n >= (idx_t)cap) {
        size_t new_cap = cap ? cap * 2 : 13;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();            /* OOM – abort is safe here          */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n (if any). */
    if ((idx_t)sz <= n) {
        for (size_t i = sz; i <= (size_t)n; ++i) {
            if (i > 12) {
                cache[i] = cache[i-1] + cache[i-11] - cache[i-12];
            }
            /* else the first 13 terms are already set            */
        }
        sz = (size_t)n + 1;
    }

    return cache[n];
}
