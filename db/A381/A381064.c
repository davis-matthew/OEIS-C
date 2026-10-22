/*
 * A381064  Expansion of e.g.f. log(1-x)^2 * exp(-x) / 2.
 * https://oeis.org/A381064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * binomial(n,k) *
 *            |Stirling1(k,2)|
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* --------------------------------------------------------------------- */
    /* Cache for |Stirling1(k,2)|                                         */
    /* --------------------------------------------------------------------- */
    static seq_t *s2 = NULL;          /* array of |s(k,2)| values          */
    static idx_t  s2_cap = 0;         /* current capacity (size)           */
    static idx_t  s2_last_n = 1;      /* last n for which we have computed  */
    static seq_t  fact_s2 = 1;        /* (s2_last_n-1)! after last compute */

    if (n >= s2_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(s2, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failed */
        s2 = tmp;
        s2_cap = new_cap;

        /* initialise first two entries if this is the first allocation   */
        if (s2_cap >= 2) {
            s2[0] = 0;
            s2[1] = 0;
        }

        /* compute new |s(k,2)| values from s2_last_n+1 up to n               */
        idx_t k_start = s2_last_n + 1;
        if (k_start < 2) k_start = 2; /* start at 2, since s2[0] and s2[1] are 0 */

        for (idx_t k = k_start; k <= n; ++k) {
            /* recurrence: s(k,2) = (k-2)! + (k-1) * s(k-1,2)                 */
            seq_t term = fact_s2 + (seq_t)(k - 1) * s2[k - 1];
            s2[k] = term;
            /* update factorial for next iteration: (k-1)! = fact_s2 * (k-1)   */
            fact_s2 *= (seq_t)(k - 1);
        }
        s2_last_n = n;
    }

    /* --------------------------------------------------------------------- */
    /* Cache for the sequence values a(m) (memoisation)                       */
    /* --------------------------------------------------------------------- */
    static seq_t *ans = NULL;         /* array of computed a(n) values      */
    static idx_t  ans_cap = 0;        /* size of ans array                  */
    static idx_t  ans_last_n = 0;     /* last n that has been computed      */

    if (n >= ans_cap) {
        size_t old_cap = ans_cap;
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(ans, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failed */
        ans = tmp;
        ans_cap = new_cap;

        /* compute a(m) for all new indices m = old_cap .. n inclusive          */
        for (idx_t m = old_cap; m <= n; ++m) {
            seq_t pos = 0;   /* sum of terms with positive sign                 */
            seq_t neg = 0;   /* sum of terms with negative sign                 */
            seq_t binom = 1; /* binomial(m,0)                                 */

            for (idx_t k = 0; k <= m; ++k) {
                seq_t term = binom * s2[k];
                if ((m - k) & 1U) {
                    neg += term;
                } else {
                    pos += term;
                }

                /* update binomial for next k+1: binom * (m-k)/(k+1)           */
                if (k < m) {
                    binom = (binom * (seq_t)(m - k)) / (seq_t)(k + 1);
                }
            }
            ans[m] = pos - neg; /* final value, guaranteed non‑negative          */
        }
        ans_last_n = n;
    }

    return ans[n];
}
