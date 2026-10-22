/*
 * A346987  Expansion of e.g.f. 1 / (1 + 5 * log(1 - x))^(1/5).
 * https://oeis.org/A346987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A346987 – Expansion of e.g.f. 1 / (1 + 5 * log(1 - x))^(1/5)
 *  Recurrence:  a(0) = 1
 *               a(n) = sum_{k=1..n} (n!/(n-k)!)*(5/k-4/n)*a(n‑k)   (n>0)
 *  Indices: a(0) is the first term.  The function works for any n >= 0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;          /* array of computed terms   */
    static idx_t cache_size = 0;         /* number of allocated slots */
    static idx_t last_computed = (idx_t)-1;  /* largest n that has been computed */

    /* grow the cache if necessary (cast to suppress C++ warning) */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();               /* allocation failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* initialise a(0) on first use */
    if (last_computed == (idx_t)-1) {
        cache[0] = (seq_t)1;            /* a(0) = 1 */
        last_computed = 0;
    }

    /* compute any missing terms up to n */
    for (idx_t m = last_computed + 1; m <= n; ++m) {
        if (m == 0) {                    /* already set */
            last_computed = 0;
            continue;
        }

        seq_t sum = 0;
        seq_t fact_factor = 1;            /* product (m-k+1)*…*m  */

        for (idx_t k = 1; k <= m; ++k) {
            /* update falling factorial: (m-k+1)*…*m */
            fact_factor *= (seq_t)(m - k + 1);

            seq_t term = cache[m - k];    /* a(m-k)            */
            seq_t coef = (seq_t)(5 * m - 4 * k);   /* 5m - 4k          */
            seq_t numerator = term * coef;        /* term*(5m-4k)      */
            numerator *= fact_factor;             /* * (m!/(m-k)!)    */
            seq_t denom = (seq_t)k * (seq_t)m;      /* k*m               */
            seq_t add = numerator / denom;        /* exact division   */
            sum += add;
        }
        cache[m] = sum;
        last_computed = m;
    }

    return cache[n];
}
