/*
 * A315850  Coordination sequence Gal.6.104.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-3) - a(n-4) + a(n-5) + a(n-7) - a(n-8) + a(n-9) -
 *            a(n-12) * Initial values: a(0)=1, a(1)=6, a(2)=8, a(3)=10,
 *            a(4)=12, a(5)=18, * a(6)=26, a(7)=26, a(8)=28, a(9)=32,
 *            a(10)=34, a(11)=44, * a(12)=46
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for memoization */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* allocated capacity (in elements) */
    static size_t  len   = 0;   /* number of elements currently computed */

    /* initial capacity and pre‑compute the first 13 terms */
    if (cache == NULL) {
        cap = 16;
#ifdef __cplusplus
        cache = new seq_t[cap];
#else
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
#endif
        /* populate known values */
        cache[0]  = (seq_t)1;
        cache[1]  = (seq_t)6;
        cache[2]  = (seq_t)8;
        cache[3]  = (seq_t)10;
        cache[4]  = (seq_t)12;
        cache[5]  = (seq_t)18;
        cache[6]  = (seq_t)26;
        cache[7]  = (seq_t)26;
        cache[8]  = (seq_t)28;
        cache[9]  = (seq_t)32;
        cache[10] = (seq_t)34;
        cache[11] = (seq_t)44;
        cache[12] = (seq_t)46;
        len = 13;
    }

    /* enlarge the cache if needed */
    if ((size_t)n >= len) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;   /* grow by factor 2 */

        seq_t *tmp;
#ifdef __cplusplus
        /* allocate new block, copy existing values, then delete old */
        tmp = new seq_t[new_cap];
        for (size_t i = 0; i < len; ++i)
            tmp[i] = cache[i];
        delete[] cache;
#else
        tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
        if (!tmp) return (seq_t)0;   /* or handle out‑of‑memory suitably */
        cache = tmp;

        /* initialise new slots (only needed for C allocation) */
#ifndef __cplusplus
        for (size_t i = len; i < new_cap; ++i)
            cache[i] = (seq_t)0;
#endif
        cap = new_cap;
        /* compute the missing terms up to n */
        for (size_t m = len; m <= (size_t)n; ++m) {
            /* recurrence: a(m) = a(m-3) - a(m-4) + a(m-5) + a(m-7)
             *                    - a(m-8) + a(m-9) - a(m-12) */
            seq_t t = cache[m - 3];
            t -= cache[m - 4];
            t += cache[m - 5];
            t += cache[m - 7];
            t -= cache[m - 8];
            t += cache[m - 9];
            t -= cache[m - 12];
            cache[m] = t;
        }
        len = (size_t)n + 1;
    }

    return cache[n];
}
