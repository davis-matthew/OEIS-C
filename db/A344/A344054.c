/*
 * A344054  a(n) = Sum_{k = 0..n} E1(n, k)*k^2, where E1 are the Eulerian
 *          numbers A173018.
 * https://oeis.org/A344054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k = 0..n} E1(n, k)*k^2 (Eulerian transform of the
 *            squares)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute A344054 */
seq_t a(idx_t n)
{
    /* Static cache of already-computed values */
    static seq_t *e = NULL;      /* current Eulerian numbers up to last computed n */
    static size_t e_cap = 0;     /* allocated capacity of e */
    static seq_t *a_cache = NULL;/* stored a(n) values */
    static size_t a_cap = 0;     /* allocated capacity of a_cache */
    static idx_t last_n = 0;     /* highest n for which a(n) is known */

    /* Ensure cache starts with n = 0 */
    if (last_n == 0 && e == NULL)
    {
        e_cap = 1;
        e = (seq_t *)malloc(e_cap * sizeof(seq_t));
        if (!e) return 0;          /* allocation failed */
        e[0] = (seq_t)1;           /* E(0,0) = 1 */

        a_cap = 1;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        if (!a_cache) return 0;
        a_cache[0] = (seq_t)0;     /* a(0) = 0 */
    }

    /* If the requested value has already been computed, return it */
    if (n <= last_n)
        return a_cache[n];

    /* Compute values up to n, extending the cache as necessary */
    for (idx_t m = last_n + 1; m <= n; ++m)
    {
        /* extend Eulerian array to size m+1 */
        if (e_cap < (size_t)(m + 1))
        {
            seq_t *tmp = (seq_t *)realloc(e, (size_t)(m + 1) * sizeof(seq_t));
            if (!tmp) return 0;
            e = tmp;
            e_cap = (size_t)(m + 1);
        }

        /* extend a-cache to size m+1 */
        if (a_cap < (size_t)(m + 1))
        {
            seq_t *tmp = (seq_t *)realloc(a_cache, (size_t)(m + 1) * sizeof(seq_t));
            if (!tmp) return 0;
            a_cache = tmp;
            a_cap = (size_t)(m + 1);
        }

        /* Number of existing Eulerian entries before this level */
        idx_t prev_size = m;   /* since e currently holds values for n=m-1 */

        /* Compute new Eulerian numbers for level m in-place, descending */
        seq_t a_m = 0;                 /* sum to obtain a(m) */
        for (idx_t k = m; ; --k)
        {
            seq_t term1 = 0, term2 = 0;

            if (k < prev_size)                     /* E[n-1][k] exists */
                term1 = (seq_t)(k + 1) * e[k];

            if (k > 0)                              /* E[n-1][k-1] exists */
                term2 = (seq_t)(m - k) * e[k - 1];

            e[k] = term1 + term2;                  /* new E[n][k] */

            /* accumulate contribution to a_m */
            if (k > 0) /* k==0 contributes 0 because k^2 = 0 */
            {
                seq_t k_sq = (seq_t)k * (seq_t)k;
                a_m += e[k] * k_sq;
            }

            if (k == 0) break;
        }

        a_cache[m] = a_m;
        last_n = m;          /* update the highest computed n */
    }

    return a_cache[n];
}
