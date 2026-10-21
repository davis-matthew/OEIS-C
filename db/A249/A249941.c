/*
 * A249941  E.g.f.: Sum_{n>=0} exp(n^3*x) / 2^(n+1).
 * https://oeis.org/A249941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000670(3*n) = Sum_{k=0..3*n} k! * Stirling2(3*n,k)
 *            (ordered Bell number of 3*n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Cache of ordered‑Bell numbers up to the current maximum index. */
    static seq_t *f = NULL;    /* array of previously computed values */
    static size_t fsize = 0;   /* largest index already stored + 1 */

    /* Target index is 3*n. */
    idx_t m_idx = 3 * n;
    size_t m = (size_t)m_idx;      /* safe for array indexing */

    /* Grow the cache if necessary. */
    if (m + 1 > fsize)
    {
        size_t new_size = m + 1;
        seq_t *tmp = (seq_t *)realloc(f, new_size * sizeof(seq_t));
        if (!tmp)
            return 0;                 /* allocation failed – return zero */

        f = tmp;

        /* First element: F(0) = 1. */
        if (fsize == 0)
        {
            f[0] = 1;
            fsize = 1;
        }

        /* Compute all new values F(i) for i = fsize … m using the recurrence
           F(i) = ∑_{k=0}^{i-1} binom(i,k) * F(k)  (binom(i,i) = 1 contributes F(i) to LHS). */
        for (size_t i = fsize; i <= m; ++i)
        {
            seq_t sum = 0;
            seq_t bin = 1;          /* binom(i,0) */

            for (size_t k = 0; k < i; ++k)
            {
                sum += bin * f[k];

                /* Update binomial coefficient for the next k. */
                bin = bin * (seq_t)(i - k);     /* multiply by (i-k) */
                bin = bin / (seq_t)(k + 1);     /* divide by (k+1) – exact division by construction */
            }

            f[i] = sum;
        }

        fsize = new_size;
    }

    return f[m];
}
