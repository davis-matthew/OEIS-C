/*
 * A385322  E.g.f. A(x) satisfies A(x) = exp(x + x^2/2 * A'(x)).
 * https://oeis.org/A385322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = a(n-1) + (1/2) * Σ_{k=0}^{n-1} (1+k) * k *
 *            C(n-1,k) * a(k) * a(n-1-k) for n ≥ 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;     /* memoized values of the sequence */
static size_t cap = 0;          /* current capacity of the cache array */
static idx_t max_cached = 0;    /* index of the largest computed value */

seq_t a(idx_t n)
{
    /* lazy initialization of the cache */
    if (cache == NULL) {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        max_cached = 0;
    }

    /* enlarge cache if necessary */
    size_t needed = (size_t)(n + 1);
    if (needed > cap) {
        size_t new_cap = cap ? cap * 2 : 1;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t m = max_cached + 1; m <= n; ++m) {
        seq_t sum = 0;
        seq_t bin = 1;                     /* C(m-1,0) */
        idx_t m_minus1 = m - 1;
        for (idx_t k = 0; k < m; ++k) {
            seq_t term = cache[k];
            term = term * cache[m_minus1 - k];
            term = term * ((seq_t)(1 + k) * (seq_t)k);
            term = term * bin;
            sum = sum + term;

            /* update binomial coefficient for next k */
            if (k + 1 < m) {
                seq_t num = (seq_t)(m_minus1 - k);
                bin = bin * num;
                bin = bin / (k + 1);
            }
        }
        seq_t half = sum / 2;
        cache[m] = cache[m - 1] + half;
    }
    max_cached = n;
    return cache[n];
}
