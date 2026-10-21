/*
 * A291974  a(n) = (3*n)! * [z^(3*n)] exp(-(exp(z)/3 +
 *          2*exp(-z/2)*cos(z*sqrt(3)/2)/3 - 1)).
 * https://oeis.org/A291974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (3*n)! * [z^(3*n)] exp(-(exp(z)/3 +
 *            2*exp(-z/2)*cos(z*sqrt(3)/2)/3 - 1)). Computed by the recurrence
 *            a(0) = 1 a(n) = - ∑_{p=1}^{n-1} binom(3n-1, 3p) * p/(n-p) *
 *            a(n-p) - a(0) where binom() is the ordinary binomial
 *            coefficient. The term for p=n has coefficient 1, hence the extra
 *            -a(0)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 10, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper to allocate or extend an array of seq_t values */
static seq_t *allocate_seq(idx_t new_cap)
{
    return (seq_t *)realloc(NULL, new_cap * sizeof(seq_t));
}

/* Global memoisation tables */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;

/* Ensure that values up to n are computed */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;                      /* already computed */

    /* increase size of cache */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    cache = new_cache;

    /* initialise from the first unused index */
    if (cache_cap == 0) {
        cache[0] = 1;                 /* a(0) */
    }

    /* compute missing entries */
    for (idx_t m = cache_cap; m <= n; ++m) {
        if (m == 0) {
            cache[0] = 1;
            continue;
        }

        /* compute a(m) by recurrence */
        seq_t sum = 0;

        /* handle the term with p = m (coeff = 1) */
        sum += cache[0];              /* a(0) * 1 */

        if (m == 1) {                 /* only the p=m term remains */
            cache[1] = -sum;
            continue;
        }

        /* initialise binomial bin = C(3m-1, 3) */
        idx_t n3m_minus1 = 3 * m - 1;
        seq_t bin = ((seq_t)n3m_minus1) * (n3m_minus1 - 1) *
                    (n3m_minus1 - 2) / 6;                /* binom(3m-1,3) */

        for (idx_t p = 1; p <= m - 1; ++p) {
            /* coefficient = bin * p / (m - p) */
            seq_t coeff = (bin * (seq_t)p) / (seq_t)(m - p);
            sum += coeff * cache[m - p];

            /* update bin to C(3m-1, 3(p+1)) */
            if (p < m - 1) {
                /* multiply by (n - 3p) / (3p + 1) */
                bin *= (seq_t)(n3m_minus1 - 3 * p);
                bin /= (seq_t)(3 * p + 1);
                /* multiply by (n - 3p - 1) / (3p + 2) */
                bin *= (seq_t)(n3m_minus1 - 3 * p - 1);
                bin /= (seq_t)(3 * p + 2);
                /* multiply by (n - 3p - 2) / (3p + 3) */
                bin *= (seq_t)(n3m_minus1 - 3 * p - 2);
                bin /= (seq_t)(3 * p + 3);
            }
        }

        cache[m] = -sum;
    }

    cache_cap = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
