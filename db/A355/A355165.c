/*
 * A355165  a(n) = exp(-1/4) * Sum_{k>=0} (4*k + 2)^n / (4^k * k!).
 * https://oeis.org/A355165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 2*a(n-1) + Σ_{k=1..n} binomial(n-1,k-1)*4^(k-1)*a(n-k)
 * (OEIS offset 0)
 * a(0) = 1
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;   /* array of computed a(i) */
    static idx_t cache_cap   = 0;      /* allocated capacity (in elements) */
    static idx_t cache_used  = 0;      /* number of elements actually stored */

    /* increase capacity if needed */
    if (n + 1 > cache_cap) {
        idx_t new_cap = (cache_cap < 2) ? (n + 1) : (cache_cap * 2);
        if (new_cap < n + 1) new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* initialise base case if required */
    if (cache_used == 0) {
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        cache_used = 1;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_used; i <= n; ++i) {
        if (i == 0) continue;   /* already set */

        /* start with 2*a(i-1) */
        seq_t sum = cache[i-1] * (seq_t)2;

        /* compute the convolution sum */
        seq_t bin   = (seq_t)1;   /* binomial(i-1,0) */
        seq_t pow4  = (seq_t)1;   /* 4^0 */

        for (idx_t j = 0; j < i; ++j) {
            /* term: bin * pow4 * a(i-1-j) */
            seq_t term = bin * pow4;
            term *= cache[i-1-j];
            sum += term;

            /* update binomial coefficient and power of 4 for next j */
            if (j + 1 < i) {
                /* bin = bin * (i-1-j) / (j+1)  */
                bin = bin * (seq_t)(i - 1 - j);
                bin = bin / (seq_t)(j + 1);
                pow4 = pow4 * (seq_t)4;
            }
        }
        cache[i] = sum;
    }

    cache_used = n + 1;
    return cache[n];
}
