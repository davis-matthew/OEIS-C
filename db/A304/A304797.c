/*
 * A304797  Expansion of x * (d/dx) Sum_{k>=0}
 *          k!*x^(k*(k+1)/2)/Product_{j=1..k} (1 - x^j).
 * https://oeis.org/A304797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*A032020(n) = n * sum_{k>=1} k! * p_distinct(n,k) where
 *            p_distinct(n,k) is the number of partitions of n into exactly k
 *            distinct parts. The value p_distinct(n,k) equals the number of
 *            ordinary partitions of n - k(k-1)/2 into exactly k parts. This
 *            code builds a DP table for ordinary partiti...
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* compute maximum number of parts needed for this n */
    idx_t kmax = 0;
    while ((kmax + 1) * (kmax + 2) / 2 <= n)
        ++kmax;
    size_t stride = (size_t)(kmax + 1);

    /* static caches */
    static seq_t *p_tbl = NULL;      /* partition table p[s][k] */
    static seq_t *fact   = NULL;      /* factorial array fact[k]   */
    static size_t stored_stride = 0;
    static idx_t  max_n_used   = 0;

    /* reallocate tables if needed (larger n or larger stride) */
    if (!p_tbl || n > max_n_used || stride > stored_stride) {
        /* free old tables */
        free(p_tbl);
        free(fact);

        /* allocate new tables */
        size_t total = (size_t)(n + 1) * stride;
        p_tbl = (seq_t *)calloc(total, sizeof(seq_t));
        if (!p_tbl) return (seq_t)0;        /* allocation failure */

        /* base case */
        p_tbl[0] = (seq_t)1;   /* p[0][0] = 1 */

        /* build ordinary partition table p[s][k] up to n and kmax */
        for (idx_t s = 1; s <= n; ++s) {
            for (idx_t k = 1; k <= kmax; ++k) {
                seq_t val = 0;
                val = p_tbl[(s - 1) * stride + (k - 1)];
                if (s >= k)
                    val = val + p_tbl[(s - k) * stride + k];
                p_tbl[s * stride + k] = val;
            }
        }

        /* build factorials up to kmax */
        fact = (seq_t *)malloc(sizeof(seq_t) * stride);
        if (!fact) { free(p_tbl); return (seq_t)0; }
        fact[0] = (seq_t)1;
        for (idx_t i = 1; i <= kmax; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;

        stored_stride = stride;
        max_n_used   = n;
    }

    /* compute the coefficient c(n) = sum_k k! * p_distinct(n,k) */
    seq_t c = (seq_t)0;
    for (idx_t k = 1; k <= kmax; ++k) {
        idx_t m = n - k * (k - 1) / 2;                    /* n - k(k-1)/2 */
        seq_t d = p_tbl[m * stride + k];                 /* p[m][k] */
        if (d)                                            /* avoid zero add */
            c = c + fact[k] * d;
    }

    /* final result: a(n) = n * c */
    return (seq_t)n * c;
}
