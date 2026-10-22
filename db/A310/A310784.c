/*
 * A310784  Coordination sequence Gal.6.72.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-5) + a(n-9) - a(n-14) for n > 18 Initial values:
 *            a(0)=1, a(1)=4, a(2)=7, a(3)=12, a(4)=17, a(5)=18, a(6)=30,
 *            a(7)=26, a(8)=36, a(9)=39, a(10)=39, a(11)=52, a(12)=47,
 *            a(13)=58, a(14)=58, a(15)=64, a(16)=70, a(17)=70, a(18)=79,
 *            a(19)=79, a(20)=86, a(21)=91, a(22)=92, a(23)=98
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* Initialised first 24 terms */
    static const seq_t init_terms[24] = {
        (seq_t)1,(seq_t)4,(seq_t)7,(seq_t)12,(seq_t)17,
        (seq_t)18,(seq_t)30,(seq_t)26,(seq_t)36,(seq_t)39,
        (seq_t)39,(seq_t)52,(seq_t)47,(seq_t)58,(seq_t)58,
        (seq_t)64,(seq_t)70,(seq_t)70,(seq_t)79,(seq_t)79,
        (seq_t)86,(seq_t)91,(seq_t)92,(seq_t)98
    };

    /* Initialise cache on first call */
    if (cache == NULL) {
        cap = 24;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < cap; ++i) {
            cache[i] = init_terms[i];
        }
    }

    /* Ensure the cache can hold up to index n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed; return 0 as safe fallback */
            return 0;
        }
        /* Initialise new slots to zero */
        for (size_t i = cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cap   = new_cap;
    }

    /* For indices already known directly */
    if (n < 24)
        return cache[(size_t)n];

    /* Compute values from the first unknown up to n */
    for (size_t i = 24; i <= (size_t)n; ++i) {
        if (cache[i] == 0) /* not yet computed */
            cache[i] = cache[i - 5] + cache[i - 9] - cache[i - 14];
    }

    return cache[(size_t)n];
}
