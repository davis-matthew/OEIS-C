/*
 * A244179  Number of length n+4 0..4 arrays with no five consecutive elements
 *          with pattern ababa or abbba (possibly a=b) and new values 0..4
 *          introduced in 0..4 order.
 * https://oeis.org/A244179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 8*a(n-2) -5*a(n-3) -36*a(n-4) -86*a(n-5)
 *            -106*a(n-6) -52*a(n-7) +102*a(n-8) +337*a(n-9) +559*a(n-10)
 *            +701*a(n-11) +701*a(n-12) +582*a(n-13) +403*a(n-14) +227*a(n-15)
 *            +105*a(n-16) +34*a(n-17) +8*a(n-18); (a(1)=49, a(2)=188,
 *            a(3)=790, a(4)=3537, a(5)=16477, a(6)=78614, a(7)=380407...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[18] = {
    (seq_t)49,  (seq_t)188,  (seq_t)790,  (seq_t)3537,
    (seq_t)16477, (seq_t)78614, (seq_t)380407, (seq_t)1855983,
    (seq_t)9098538, (seq_t)44726540, (seq_t)220215471,
    (seq_t)1085242000, (seq_t)5350977611, (seq_t)26391893977,
    (seq_t)130191662653, (seq_t)642301565508, (seq_t)3168980716083,
    (seq_t)15635596759403
};

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is the first term. */
    if (n < 1) return (seq_t)0;

    /* Static cache to store computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Initialize cache with the first 18 terms on first call. */
    if (cache_len == 0) {
        cache_len = 18;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return (seq_t)0; /* Allocation failed */
        for (size_t i = 0; i < cache_len; ++i) cache[i] = init[i];
    }

    /* If requested term is already in cache, return it. */
    if ((size_t)n <= cache_len) return cache[n - 1];

    /* Extend cache up to the requested index. */
    size_t new_len = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (!tmp) return (seq_t)0; /* Allocation failed */
    cache = tmp;

    /* Compute new terms using the 18‑term linear recurrence. */
    for (size_t i = cache_len; i < new_len; ++i) {
        /* i indexes 0‑based, corresponding to a(i+1) */
        seq_t val = cache[i - 1] * (seq_t)4 + cache[i - 2] * (seq_t)8;
        val += cache[i - 8] * (seq_t)102;
        val += cache[i - 9] * (seq_t)337;
        val += cache[i -10] * (seq_t)559;
        val += cache[i -11] * (seq_t)701;
        val += cache[i -12] * (seq_t)701;
        val += cache[i -13] * (seq_t)582;
        val += cache[i -14] * (seq_t)403;
        val += cache[i -15] * (seq_t)227;
        val += cache[i -16] * (seq_t)105;
        val += cache[i -17] * (seq_t)34;
        val += cache[i -18] * (seq_t)8;
        val -= cache[i - 3] * (seq_t)5;
        val -= cache[i - 4] * (seq_t)36;
        val -= cache[i - 5] * (seq_t)86;
        val -= cache[i - 6] * (seq_t)106;
        val -= cache[i - 7] * (seq_t)52;
        cache[i] = val;
    }
    cache_len = new_len;
    return cache[n - 1];
}
