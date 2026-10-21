/*
 * A247125  Number of tilings of a 5 X n rectangle using n pentominoes of
 *          shapes L, U, X.
 * https://oeis.org/A247125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2)+a(n-3)+12*a(n-4)+6*a(n-5)+2*a(n-6) with a(0)=1,
 *            a(1)=0, a(2)=2, a(3)=1, a(4)=16, a(5)=10
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 54, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent cache of already computed values */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;   /* allocated length of cache   */
    static size_t  len    = 0;   /* number of filled elements   */

    /* Ensure the cache is large enough for index n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        if (new_cap < 8) new_cap = 8;          /* initial minimal size   */
        while (new_cap < (size_t)n + 1)        /* grow exponentially    */
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* out‑of‑memory: return 0 */
        cache = tmp;
        cap = new_cap;
    }

    /* Initialize basic values if not yet done */
    if (len == 0) {
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)0;   /* a(1) */
        cache[2] = (seq_t)2;   /* a(2) */
        cache[3] = (seq_t)1;   /* a(3) */
        cache[4] = (seq_t)16;  /* a(4) */
        cache[5] = (seq_t)10;  /* a(5) */
        len = 6;
    }

    /* Compute missing values up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t val = 0;

        if (i >= 2) val += (seq_t)2 * cache[i - 2];
        if (i >= 3) val +=              cache[i - 3];
        if (i >= 4) val += (seq_t)12 * cache[i - 4];
        if (i >= 5) val += (seq_t)6  * cache[i - 5];
        if (i >= 6) val += (seq_t)2  * cache[i - 6];

        cache[i] = val;
    }
    len = (size_t)n + 1;
    return cache[(size_t)n];
}
