/*
 * A305512  Number of nX4 0..1 arrays with every element unequal to 1, 2, 5, 6
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            a(n-1)+a(n-2)+5*a(n-3)-8*a(n-6)-8*a(n-7)-10*a(n-9)-a(n-10)+a(n-11)-2*a(n-12)-2*a(n-14)
 *            Initial values (n = 1..17): 2, 1, 6, 10, 13, 38, 73, 164, 351,
 *            749, 1719, 3710, 8182, 18453, 40666, 90453, 201921
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 57, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache          = NULL;
    static size_t cache_cap      = 0;    /* allocated capacity */
    static size_t cache_len      = 0;    /* number of values stored (== highest computed n) */

    /* Initialise cache with the first 17 terms on first call */
    if (cache == NULL) {
        /* Reserve a bit more to avoid frequent reallocs */
        cache_cap = 32;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) return 0;    /* out of memory */

        cache[0]  = 2;
        cache[1]  = 1;
        cache[2]  = 6;
        cache[3]  = 10;
        cache[4]  = 13;
        cache[5]  = 38;
        cache[6]  = 73;
        cache[7]  = 164;
        cache[8]  = 351;
        cache[9]  = 749;
        cache[10] = 1719;
        cache[11] = 3710;
        cache[12] = 8182;
        cache[13] = 18453;
        cache[14] = 40666;
        cache[15] = 90453;
        cache[16] = 201921;
        cache_len = 17;
    }

    /* Return pre‑computed value if it already exists */
    if ((size_t)n <= cache_len)
        return cache[n-1];

    /* Ensure capacity for all needed terms up to n */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;      /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new terms sequentially using the recurrence */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        seq_t v = cache[i-1];                     /* a(n-1) */
        v += cache[i-2];                          /* + a(n-2) */
        v += cache[i-3] * 5;                      /* + 5*a(n-3) */
        v -= cache[i-6] * 8;                      /* - 8*a(n-6) */
        v -= cache[i-7] * 8;                      /* - 8*a(n-7) */
        v -= cache[i-9] * 10;                     /* - 10*a(n-9) */
        v -= cache[i-10];                         /* - a(n-10) */
        v += cache[i-11];                         /* + a(n-11) */
        v -= cache[i-12] * 2;                     /* - 2*a(n-12) */
        v -= cache[i-14] * 2;                     /* - 2*a(n-14) */
        cache[i] = v;
    }

    cache_len = (size_t)n;
    return cache[n-1];
}
