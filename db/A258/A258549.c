/*
 * A258549  Number of (n+1) X (3+1) 0..1 arrays with every 2 X 2 subblock
 *          ne-sw antidiagonal difference nondecreasing horizontally and nw+se
 *          diagonal sum nondecreasing vertically.
 * https://oeis.org/A258549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - 4*a(n-5) + 6*a(n-6) -
 *            4*a(n-7) + a(n-8); n > 11 Initial values: a(1) = 104, a(2)=210,
 *            a(3)=598, a(4)=1244, a(5)=2384, a(6)=3980, a(7)=6348, a(8)=9567,
 *            a(9)=13847, a(10)=19227, a(11)=26071
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------ */
    /* persistent cache – memoises already-computed values                */
    /* ------------------------------------------------------------------ */
    static seq_t *cache   = NULL;   /* array indexed 0..k, cache[0] unused   */
    static idx_t  cache_len = 0;    /* largest index currently stored      */

    /* ------------------------------------------------------------------ */
    /* initial values up to n=11 (cached only once)                       */
    /* ------------------------------------------------------------------ */
    static const seq_t init_vals[12] = {
        0, 104, 210, 598, 1244, 2384, 3980,
        6348, 9567, 13847, 19227, 26071
    };

    /* first call: allocate and fill the cache with the initial values   */
    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)12 * sizeof(seq_t));
        for (idx_t i = 0; i < 12; ++i)
            cache[i] = init_vals[i];
        cache_len = 11;   /* indices 1..11 are now available              */
    }

    /* ------------------------------------------------------------------ */
    /* if the requested index is already cached, return the stored value*/
    /* ------------------------------------------------------------------ */
    if (n <= cache_len)
        return cache[n];

    /* ------------------------------------------------------------------ */
    /* extend the cache to cover indices up to n                          */
    /* ------------------------------------------------------------------ */
    idx_t new_len = n;
    cache = (seq_t *)realloc(cache,
                             (size_t)(new_len + 1) * sizeof(seq_t));

    /* compute all new entries using the linear recurrence               */
    for (idx_t i = cache_len + 1; i <= new_len; ++i) {
        if (i <= 11) {            /* already initialised */
            cache[i] = init_vals[i];
            continue;
        }
        /* positives first – ensures that no intermediate underflow       */
        seq_t val = cache[i-1] * 4;          /* 4*a(i-1) */
        val +=  cache[i-3] * 4;              /* +4*a(i-3)*/
        val +=  cache[i-6] * 6;              /* +6*a(i-6)*/
        val +=  cache[i-8];                  /* +a(i-8)  */
        val -=  cache[i-2] * 6;              /* -6*a(i-2)*/
        val -=  cache[i-5] * 4;              /* -4*a(i-5)*/
        val -=  cache[i-7] * 4;              /* -4*a(i-7)*/
        cache[i] = val;
    }

    cache_len = new_len;
    return cache[n];
}
