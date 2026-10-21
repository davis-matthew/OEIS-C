/*
 * A258725  Number of length n+3 0..3 arrays with at most one downstep in
 *          every 3 consecutive neighbor pairs.
 * https://oeis.org/A258725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 20*a(n-3) - 34*a(n-4) + 24*a(n-5) -
 *            16*a(n-6) + 8*a(n-7) - a(n-8) for n > 8, with a(1)=190,
 *            a(2)=608, a(3)=2028, a(4)=6552, a(5)=20955, a(6)=68120,
 *            a(7)=220854, a(8)=711432
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 15, u64 n <= 34, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_max = 0;
    /* Initial values: 1‑based index, cache[0] unused */
    static const seq_t init[9] = {
        0ULL,          /* dummy for index 0 */
        190ULL,        /* a(1) */
        608ULL,        /* a(2) */
        2028ULL,       /* a(3) */
        6552ULL,       /* a(4) */
        20955ULL,      /* a(5) */
        68120ULL,      /* a(6) */
        220854ULL,     /* a(7) */
        711432ULL      /* a(8) */
    };

    /* Ensure we have computed up to n */
    if (n > cached_max) {
        idx_t new_max = n;
        size_t new_size = (size_t)new_max + 1;        /* account for cache[0] */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                            /* allocation failure */
        cache = tmp;
        if (cached_max == 0) cache[0] = (seq_t)0;      /* unused slot */

        for (idx_t i = cached_max + 1; i <= new_max; ++i) {
            if (i <= 8) {
                cache[i] = init[i];
            } else {
                seq_t t1 = cache[i - 1];
                seq_t t2 = cache[i - 2];
                seq_t t3 = cache[i - 3];
                seq_t t4 = cache[i - 4];
                seq_t t5 = cache[i - 5];
                seq_t t6 = cache[i - 6];
                seq_t t7 = cache[i - 7];
                seq_t t8 = cache[i - 8];

                cache[i] = (seq_t)4 * t1
                           - (seq_t)6 * t2
                           + (seq_t)20 * t3
                           - (seq_t)34 * t4
                           + (seq_t)24 * t5
                           - (seq_t)16 * t6
                           + (seq_t)8 * t7
                           - t8;
            }
        }
        cached_max = new_max;
    }

    return cache[n];
}
