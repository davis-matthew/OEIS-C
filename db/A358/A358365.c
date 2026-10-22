/*
 * A358365  a(n) = 16^n * Sum_{k=0..n} binomial(-1/2, k)^2.
 * https://oeis.org/A358365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 16*a(n-1) + binom(2*n, n)^2 (for all n >= 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use a simple incremental recurrence based on the identity
   binom(-1/2, k) = (-1)^k * binom(2k, k) / 4^k.
   Then  a(n) = 16^n * sum_{k=0}^n [binom(-1/2,k)]^2
            = sum_{k=0}^n binom(2k, k)^2 * 16^{n-k} ,
   which leads to the recurrence above. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for already computed values */
    static seq_t *cache   = NULL;
    static unsigned char *computed = NULL;
    static idx_t cache_len = 0;

    /* Keep the last computed binomial value binom(2*m, m) */
    static seq_t prev_binom = (seq_t)1;   /* binom(0,0) */
    static idx_t prev_binom_n = 0;        /* corresponding n */

    /* Grow the cache if needed */
    if (n >= cache_len) {
        idx_t new_len = cache_len ? cache_len << 1 : n + 1;
        if (new_len < n + 1) new_len = n + 1;

        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        computed = (unsigned char *)realloc(computed,
                                           new_len * sizeof(unsigned char));
        for (idx_t i = cache_len; i < new_len; ++i)
            computed[i] = 0;
        cache_len = new_len;
    }

    /* Return immediately if already computed */
    if (computed[n])
        return cache[n];

    /* Base case */
    if (!computed[0]) {
        cache[0] = (seq_t)1;
        computed[0] = 1;
    }

    /* Compute values from the first missing index up to n */
    idx_t start = (prev_binom_n + 1) > 1 ? prev_binom_n + 1 : 1;
    for (idx_t i = start; i <= n; ++i) {

        /* Update binomial value:
           binom(2*i, i) = binom(2*(i-1), i-1) * (2*i*(2*i-1)) / (i*i)  */
        seq_t b = prev_binom * (seq_t)(2 * i) / (seq_t)i;
        b = b * (seq_t)(2 * i - 1) / (seq_t)i;
        prev_binom = b;
        prev_binom_n = i;

        /* Compute the new sequence value */
        seq_t bi2 = b * b;                     /* binomial squared */
        seq_t prev_val = cache[i - 1];
        seq_t new_val = prev_val * (seq_t)16 + bi2;

        cache[i] = new_val;
        computed[i] = 1;
    }

    return cache[n];
}
