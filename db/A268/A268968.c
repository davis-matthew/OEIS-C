/*
 * A268968  Number of n X 5 0..2 arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two not
 *          more than once.
 * https://oeis.org/A268968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30*a(n-1) - 261*a(n-2) + 540*a(n-3) - 324*a(n-4) for n≥5,
 *            with a(1)=144, a(2)=3312, a(3)=63792, a(4)=1125360. The function
 *            memoises values to be efficient for repeated calls
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n<1, but harmless */

    /* static cache: index 1.. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  max_compute = 0;   /* highest n computed */

    /* initialise cache if needed */
    if (cap == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        /* set base values */
        cache[0] = 0;
        cache[1] = 144;
        cache[2] = 3312;
        cache[3] = 63792;
        cache[4] = 1125360;
        max_compute = 4;
    }

    /* grow cache to include n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms */
    if (n > max_compute) {
        for (idx_t i = max_compute + 1; i <= n; ++i) {
            cache[i] =   30 * cache[i-1]
                       - 261 * cache[i-2]
                       + 540 * cache[i-3]
                       - 324 * cache[i-4];
        }
        max_compute = n;
    }

    return cache[n];
}
