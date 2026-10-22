/*
 * A345652  Expansion of the e.g.f. exp(-1 + (1 + x)*exp(-x)).
 * https://oeis.org/A345652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n-2} (n-1)*binomial(n-2,k)*(-1)^(n-1-k)*a(k),
 *            with a(0)=1, a(1)=0
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 15, i64 n <= 26, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/*------------------------------------------------------------*/
/*  Helper: compute binomial coefficient C(n,k) in seq_t form  */
static seq_t binomial_seq(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i)
    {
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}
/*------------------------------------------------------------*/

static seq_t *cache = NULL;      /* previously computed a(n) values */
static idx_t  max_computed = 0;  /* largest n for which cache[n] is valid */
static size_t cap = 0;           /* current allocated capacity of cache */

/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Ensure cache has space for index n */
    if (cache == NULL)
    {
        cap = (n + 1 > (idx_t)2 ? n + 1 : (idx_t)2);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;          /* a(0) */
        cache[1] = (seq_t)0;          /* a(1) */
        max_computed = (idx_t)1;
    }
    else if ((idx_t)(n + 1) > cap)
    {
        cap = n + 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    /* Compute missing values up to n */
    for (idx_t m = max_computed + (idx_t)1; m <= n; ++m)
    {
        if (m <= 1) { /* already initialized */
            continue;
        }
        /* Compute a(m) using the recurrence */
        seq_t sum = (seq_t)0;
        idx_t t = m - (idx_t)2;          /* C(m-2, k) */
        seq_t binom = (seq_t)1;          /* binomial for k=0 */
        for (idx_t k = (idx_t)0; k <= t; ++k)
        {
            /* sign: (-1)^(m-1-k) */
            if ( ((m - (idx_t)1 - k) & (idx_t)1) == 0 )
                sum += binom * cache[k];
            else
                sum -= binom * cache[k];

            /* Update binomial to C(t, k+1) if next k exists */
            if (k < t)
            {
                seq_t num = (seq_t)(t - k);
                seq_t den = (seq_t)(k + (idx_t)1);
                binom = (binom * num) / den;
            }
        }
        cache[m] = (m - (idx_t)1) * sum;
    }

    max_computed = n;
    return cache[n];
}
/*------------------------------------------------------------*/
