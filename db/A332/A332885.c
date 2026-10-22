/*
 * A332885  a(0) = a(1) = 1; a(n) = a(n-2) + Sum_{k=0..n-2} binomial(n-2,k) *
 *          a(k).
 * https://oeis.org/A332885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = 1; a(n) = a(n‑2) + Σ_{k=0..n-2} binomial(n-2,k) *
 *            a(k)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* dynamic array of computed terms */
    static size_t len   = 0;      /* number of initialized terms in cache */
    static size_t cap   = 0;      /* current allocated capacity */

    /* Initialise when called for the first time */
    if (cache == NULL) {
        cap = 4;                 /* start with a small capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;    /* memory failure – should not happen in tests */

        /* base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        len = 2;
    }

    /* Try to extend the cache if needed */
    if ((idx_t)cap <= n) {
        size_t newcap = cap * 2;
        if (newcap <= (size_t)n) newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;      /* realloc failure */
        cache = tmp;
        cap = newcap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        if (i == 0 || i == 1) {
            /* already initialised */
        } else {
            idx_t m = i - 2;               /* upper limit for the sum */
            seq_t sum = 0;
            seq_t bin = 1;                 /* binomial(m,0) */
            for (idx_t k = 0; k <= m; ++k) {
                sum = sum + bin * cache[k];
                if (k < m) {
                    /* Update bin to binomial(m,k+1) */
                    seq_t numerator   = (seq_t)(m - k);
                    seq_t denominator = (seq_t)(k + 1);
                    bin = (bin * numerator) / denominator;
                }
            }
            cache[i] = cache[i - 2] + sum;
        }
    }

    /* Update the number of initialised terms */
    if ((idx_t)len <= n) len = n + 1;

    return cache[n];
}
