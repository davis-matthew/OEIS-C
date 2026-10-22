/*
 * A300641  Number of nX3 0..1 arrays with every element equal to 2, 3, 4 or 5
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A300641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+4*a(n-2)-a(n-3)+7*a(n-4)+6*a(n-5) *
 *            -2*a(n-6)-7*a(n-7)-11*a(n-8)-12*a(n-9) *
 *            -8*a(n-10)-9*a(n-11)-3*a(n-12)-2*a(n-14)+a(n-15) * Initial
 *            values: * a(1)=0, a(2)=2, a(3)=2, a(4)=10, a(5)=30, a(6)=104, *
 *            a(7)=340, a(8)=1144, a(9)=3795, a(10)=12683, a(11)=42227, *
 *            a(12)=140876, a(13)=4...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle invalid indices */
    if (n < 1)
        return (seq_t)0;

    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* number of terms cached */

    /* initial 15 terms, 1‑based indexing in cache */
    static const seq_t init[15] = {
        (seq_t)0,  /* a(1) */
        (seq_t)2,  /* a(2) */
        (seq_t)2,  /* a(3) */
        (seq_t)10, /* a(4) */
        (seq_t)30, /* a(5) */
        (seq_t)104,/* a(6) */
        (seq_t)340,/* a(7) */
        (seq_t)1144,/* a(8) */
        (seq_t)3795,/* a(9) */
        (seq_t)12683,/* a(10) */
        (seq_t)42227,/* a(11) */
        (seq_t)140876,/* a(12) */
        (seq_t)469517,/* a(13) */
        (seq_t)1565586,/* a(14) */
        (seq_t)5219130  /* a(15) */
    };

    /* extend cache if needed */
    if (n > cache_size) {
        size_t new_cap = (size_t)n + 1;          /* allocate up to index n */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – abort gracefully */
            abort();
        }
        cache = new_cache;

        /* compute missing terms */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i <= 15) {
                cache[i] = init[i - 1];
            } else {
                seq_t sum = 0;
                sum += (seq_t)2 * cache[i - 1];
                sum += (seq_t)4 * cache[i - 2];
                sum -= cache[i - 3];
                sum += (seq_t)7 * cache[i - 4];
                sum += (seq_t)6 * cache[i - 5];
                sum -= (seq_t)2 * cache[i - 6];
                sum -= (seq_t)7 * cache[i - 7];
                sum -= (seq_t)11 * cache[i - 8];
                sum -= (seq_t)12 * cache[i - 9];
                sum -= (seq_t)8 * cache[i - 10];
                sum -= (seq_t)9 * cache[i - 11];
                sum -= (seq_t)3 * cache[i - 12];
                sum -= (seq_t)2 * cache[i - 14];
                sum += cache[i - 15];
                cache[i] = sum;
            }
        }
        cache_size = n;
    }

    return cache[n];
}
