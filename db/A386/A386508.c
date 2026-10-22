/*
 * A386508  a(0) = 1; a(n) = a(n-1) + Sum_{k=0..n-1} (1 + k) * k^5 *
 *          binomial(n-1,k) * a(k) * a(n-1-k).
 * https://oeis.org/A386508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A386508: a(0)=1;
   a(n)=a(n-1)+∑_{k=0}^{n-1} (1+k)*k^5 * binomial(n-1,k) * a(k) * a(n-1-k)   (n≥1) */

#include <stdlib.h>     /* malloc, free */
#include <stddef.h>     /* size_t */
#include "../utils/binomial.h"  /* seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* static cache to store computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of values stored */
    static idx_t cache_cap = 0;   /* current capacity of cache */

    /* ensure cache is allocated and has enough capacity for n+1 entries */
    if (cache == NULL) {
        cache_cap = (n + 1 > 128 ? n + 1 : 128);   /* start with at least 128 entries */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache_len = 0;
    } else if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap < n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        for (idx_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
        free(cache);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values up to n if not done yet */
    while (cache_len <= n) {
        idx_t i = cache_len;
        seq_t val = (i == 0) ? (seq_t)1 : cache[i - 1];  /* start with a(i-1) */

        if (i >= 1) {
            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1;            /* binomial(i-1,0) */
            idx_t im1 = i - 1;

            for (idx_t k = 0; k < i; ++k) {
                if (k > 0) {   /* k^5 is zero when k==0, skip term */
                    /* compute k^5 */
                    seq_t kseq = (seq_t)k;
                    seq_t k2   = kseq * kseq;      /* k^2 */
                    seq_t k4   = k2 * k2;          /* k^4 */
                    seq_t k5   = k4 * kseq;        /* k^5 */

                    /* coefficient (1+k)*k^5 */
                    seq_t coeff = ((seq_t)(k + 1)) * k5;

                    /* term = a(k) * a(i-1-k) * binom * coeff */
                    seq_t term = cache[k] * cache[im1 - k];
                    term = term * binom;
                    term = term * coeff;

                    sum = sum + term;
                }

                /* update binom to binomial(i-1, k+1) if needed */
                if (k + 1 < i) {
                    binom = binom * (seq_t)(im1 - k);
                    binom = binom / (seq_t)(k + 1);
                }
            }
            val = val + sum;
        }

        cache[i] = val;
        ++cache_len;
    }

    return cache[n];
}
