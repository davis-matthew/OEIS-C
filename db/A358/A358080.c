/*
 * A358080  Expansion of e.g.f. 1/(1 - x^2 * exp(x)).
 * https://oeis.org/A358080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n! * Sum_{k=0..floor(n/2)} k^(n - 2*k)/(n - 2*k)!
 *
 *  Derived recurrence:
 *      a(0) = 1
 *      a(1) = 0
 *      a(n) = n*(n-1) * Σ_{k=0}^{n-2} binom(n-2, k) * a(k),   n ≥ 2
 *  where binom(m, k) is the binomial coefficient “m choose k”.
 *
 *  This function computes a(n) for any n ≥ 0, memoising computed
 *  values so that calls with arbitrary ordering are efficient.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;  /* memoisation array */
    static idx_t  used    = 0;     /* number of entries currently valid (0 .. used-1) */
    static idx_t  cached  = 0;     /* capacity of the memoisation array    */

    /* Expand the cache if required */
    if (n >= cached)
    {
        idx_t new_cap = n + 1;                    /* we need indices 0..n inclusive */
        seq_t *new_mem = (seq_t *)malloc(sizeof(seq_t) * (size_t)new_cap);
        if (!new_mem) return 0;                   /* allocation failed */

        /* Copy old values (if any) */
        if (cache)
        {
            for (idx_t i = 0; i < used; ++i) new_mem[i] = cache[i];
            free(cache);
        }

        cache  = new_mem;
        cached = new_cap;

        /* Compute any newly required terms */
        for (idx_t i = used; i <= n; ++i)
        {
            if (i == 0)
            {
                cache[0] = 1;
            }
            else if (i == 1)
            {
                cache[1] = 0;
            }
            else    /* i ≥ 2 */
            {
                /* sum = Σ_{k=0}^{i-2} binom(i-2, k) * a(k) */
                seq_t sum   = 0;
                seq_t binom = 1;                /* binom(i-2, 0) = 1 */

                idx_t m = i - 2;
                for (idx_t k = 0; k <= m; ++k)
                {
                    /* term = binom * a(k) */
                    seq_t term = binom * cache[k];
                    sum += term;

                    /* Prepare binom for next k */
                    if (k < m)
                    {
                        /* binom(i-2, k+1) = binom(i-2, k) * (m-k) / (k+1) */
                        seq_t numer = binom * (seq_t)(m - k);
                        seq_t denom = (k + 1);
                        binom = numer / denom;     /* division is exact */
                    }
                }

                /* a(i) = i*(i-1) * sum */
                cache[i] = ((seq_t)i * (i - 1)) * sum;
            }
        }

        used = n + 1;
    }

    return cache[n];
}
