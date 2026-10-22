/*
 * A312411  Coordination sequence Gal.3.39.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = - a(n-1) + a(n-4) + 2*a(n-5) + a(n-6) - a(n-9) - a(n-10)
 *            for n > 10 Initial values: a(0)=1, a(1)=4, a(2)=8, a(3)=14,
 *            a(4)=18, a(5)=20, a(6)=26, a(7)=30, a(8)=38, a(9)=36, a(10)=44
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* dynamic cache of computed values   */
    static size_t cap   = 0;             /* current capacity of cache         */
    static size_t ready = 0;             /* number of computed elements [0..ready-1] */

    /* Ensure the cache can hold indices up to n. */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp;
#ifdef __cplusplus
        tmp = new seq_t[new_cap];
        if (cache) {
            for (size_t i = 0; i < cap; ++i) tmp[i] = cache[i];
            delete[] cache;
        }
#else
        tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
        if (!tmp) return 0; /* allocation failed – return 0 as a safe fallback */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute terms that are not yet in the cache. */
    size_t i = ready;
    if (i == 0) i = 0;                 /* start from the beginning for the first call */
    for (; i <= (size_t)n; ++i) {
        if (i <= 10) {                 /* initial values */
            switch ((int)i) {
                case 0: cache[i] = 1;  break;
                case 1: cache[i] = 4;  break;
                case 2: cache[i] = 8;  break;
                case 3: cache[i] = 14; break;
                case 4: cache[i] = 18; break;
                case 5: cache[i] = 20; break;
                case 6: cache[i] = 26; break;
                case 7: cache[i] = 30; break;
                case 8: cache[i] = 38; break;
                case 9: cache[i] = 36; break;
                case 10: cache[i] = 44;break;
            }
        } else {                       /* recurrence for n > 10 */
            seq_t val = cache[i-4];
            val += (seq_t)2 * cache[i-5];
            val += cache[i-6];
            val -= cache[i-1];
            val -= cache[i-9];
            val -= cache[i-10];
            cache[i] = val;
        }
    }
    ready = (size_t)n + 1;             /* now all indices up to n are ready   */
    return cache[(size_t)n];
}
