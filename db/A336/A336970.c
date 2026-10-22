/*
 * A336970  G.f. A(x) satisfies: A(x) = 1 - x^2 * A(x/(1 - x)) / (1 - x).
 * https://oeis.org/A336970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1, a(1) = 0, a(n) = -Sum_{k=0..n-2} binomial(n-2,k) *
 *            a(k) (n >= 2)
 * Range:     i8 n <= 9, i16 n <= 13, i32 n <= 20, i64 n <= 32, i128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache   = NULL;   /* allocated memory */
    static size_t sz      = 0;      /* number of filled elements */
    static size_t cap     = 0;      /* current capacity */

    /* grow the cache if needed */
    if (cap < (size_t)(n + 1)) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute all missing terms up to n */
    for (idx_t i = sz; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
        } else if (i == 1) {
            cache[1] = 0;
        } else {
            idx_t m = i - 2;        /* we need S_m = sum_{k=0..m} binom(m,k)*a[k] */
            seq_t sum = 0;
            seq_t bin = 1;          /* binom(m,0) */

            for (idx_t k = 0; k <= m; ++k) {
                sum += bin * cache[k];
                if (k < m) {
                    /* update bin to binom(m,k+1) */
                    seq_t num = (seq_t)(m - k);
                    seq_t den = (seq_t)(k + 1);
                    bin = (bin * num) / den;   /* exact integer division */
                }
            }
            cache[i] = -sum;
        }
    }
    sz = (size_t)(n + 1);
    return cache[n];
}
