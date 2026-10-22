/*
 * A385940  a(0) = 1; a(n) = Sum_{k=0..n-1} (1 + k) * (1 + k^3) *
 *          binomial(n-1,k) * a(k) * a(n-1-k).
 * https://oeis.org/A385940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} (1 + k)*(1 +
 *            k^3)*binomial(n-1,k)*a(k)*a(n-1-k), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoised values */
static idx_t  cache_size = 0;        /* allocated size   */
static idx_t  last_computed = 0;     /* highest index computed */

seq_t a(idx_t n)
{
    /* initialise first term */
    if (cache == NULL) {
        cache_size = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) return 0;     /* allocation failed */
        cache[0] = 1;
        last_computed = 0;
    }
    if (n == 0) return 1;

    /* grow cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0; /* allocation failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i == 0) { cache[0] = 1; continue; }

        idx_t m = i - 1;                 /* n-1 in the formula */
        seq_t sum = 0;
        seq_t binom = 1;                 /* binomial( m , 0 ) */

        for (idx_t k = 0; k <= m; ++k) {
            seq_t kseq = (seq_t)k;
            seq_t k3   = kseq * kseq * kseq;
            seq_t factor = (kseq + 1) * (k3 + 1);

            seq_t prod = cache[k] * cache[m - k];
            prod = prod * binom;
            prod = prod * factor;

            sum = sum + prod;

            /* update binomial coefficient to next k */
            if (k < m) {
                binom = binom * (m - k);      /* numerator */
                binom = binom / (k + 1);      /* denominator */
            }
        }
        cache[i] = sum;
        last_computed = i;
    }
    return cache[n];
}
