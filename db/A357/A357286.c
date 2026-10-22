/*
 * A357286  a(n) = (1/8)*A357285.
 * https://oeis.org/A357286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n = 0 … 6 a(7) = 1 a(n) = 4*a(n-1) - 3*a(n-2) -
 *            2*a(n-3) - 6*a(n-4) + 8*a(n-5) + 24*a(n-6) - 40*a(n-7) +
 *            16*a(n-8) for n ≥ 8
 * Range:     u8 n <= 12, u16 n <= 19, u32 n <= 35, u64 n <= 36, u128 n <= 36,
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
    static size_t  cap   = 0;

    /* make sure we have space for indices 0 … n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap > 0 ? cap : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed, return 0 */
            if (cache) free(cache); /* free old cache */
            cap = 0;
            cache = NULL;
            return (seq_t)0;
        }
        /* zero‑initialize new slots */
        for (size_t i = cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cap   = new_cap;

        /* initialise the first known values */
        cache[0] = 0; cache[1] = 0; cache[2] = 0;
        cache[3] = 0; cache[4] = 0; cache[5] = 0;
        cache[6] = 0; cache[7] = 1;
    }

    /* for indices ≤7 the values are known */
    if ((size_t)n <= 7) return cache[n];

    /* compute missing terms using the 8‑term linear recurrence */
    for (idx_t i = 8; i <= n; ++i) {
        seq_t v = 0;
        v += (seq_t)4  * cache[i - 1];
        v -= (seq_t)3  * cache[i - 2];
        v -= (seq_t)2  * cache[i - 3];
        v -= (seq_t)6  * cache[i - 4];
        v += (seq_t)8  * cache[i - 5];
        v += (seq_t)24 * cache[i - 6];
        v -= (seq_t)40 * cache[i - 7];
        v += (seq_t)16 * cache[i - 8];
        cache[i] = v;
    }

    return cache[n];
}
