/*
 * A304414  Number of nX3 0..1 arrays with every element unequal to 0, 2, 3, 4
 *          or 5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A304414: number of n×3 binary arrays with upper‑left element 0,
   and no two king‑move adjacent elements being equal.
   Empirical linear recurrence (order 15, for n > 17):
   a(n) = 5*a(n-1) + 7*a(n-2) - 16*a(n-3) - 78*a(n-4) - 11*a(n-5)
          + 133*a(n-6) + 161*a(n-7) + 58*a(n-8)
          - 114*a(n-9) - 121*a(n-10) - 91*a(n-11) - 47*a(n-12) - 12*a(n-13)
          + 4*a(n-14) + 8*a(n-15).
   The first 17 terms (n = 1…17) are hard‑coded as base cases.*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* number of values stored (including index 0) */

    /* safety check – the sequence is defined only for n ≥ 1 */
    if (n == 0) return 0;

    /* ensure the cache is large enough for indices 0…n */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;   /* indices 0 … n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure */
        cache = tmp;

        /* initialise base cases a(1)…a(17); a(0) is unused but kept 0 */
        static const seq_t init[18] = {
            (seq_t)0ULL,          /* index 0, unused */
            (seq_t)1ULL,          /* a(1)  */
            (seq_t)8ULL,          /* a(2)  */
            (seq_t)23ULL,         /* a(3)  */
            (seq_t)103ULL,        /* a(4)  */
            (seq_t)520ULL,        /* a(5)  */
            (seq_t)2671ULL,       /* a(6)  */
            (seq_t)13876ULL,      /* a(7)  */
            (seq_t)72399ULL,      /* a(8)  */
            (seq_t)378321ULL,     /* a(9)  */
            (seq_t)1979531ULL,    /* a(10) */
            (seq_t)10361881ULL,   /* a(11) */
            (seq_t)54254622ULL,   /* a(12) */
            (seq_t)284118654ULL,  /* a(13) */
            (seq_t)1487964150ULL, /* a(14) */
            (seq_t)7792975441ULL, /* a(15) */
            (seq_t)40815315514ULL,/* a(16) */
            (seq_t)213770499606ULL/* a(17) */
        };

        /* fill newly allocated entries */
        for (size_t i = cache_len; i <= n; ++i) {
            if (i <= 17) {
                cache[i] = init[i];
            } else {
                /* recurrence for n > 17 */
                cache[i] =
                    ((seq_t)5)  * cache[i-1] +
                    ((seq_t)7)  * cache[i-2] -
                    ((seq_t)16) * cache[i-3] -
                    ((seq_t)78) * cache[i-4] -
                    ((seq_t)11) * cache[i-5] +
                    ((seq_t)133)* cache[i-6] +
                    ((seq_t)161)* cache[i-7] +
                    ((seq_t)58) * cache[i-8] -
                    ((seq_t)114)* cache[i-9] -
                    ((seq_t)121)* cache[i-10] -
                    ((seq_t)91) * cache[i-11] -
                    ((seq_t)47) * cache[i-12] -
                    ((seq_t)12) * cache[i-13] +
                    ((seq_t)4)  * cache[i-14] +
                    ((seq_t)8)  * cache[i-15];
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
