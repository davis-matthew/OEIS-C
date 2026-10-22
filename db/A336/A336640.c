/*
 * A336640  a(n) is the minimal value of Sum x_i when Sum binomial(x_i, 2) =
 *          n.
 * https://oeis.org/A336640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = minimal Σx_i such that Σ binomial(x_i,2) = n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A336640)                                              */
#include <stdlib.h>
#include <stdbool.h>

/* Minimal sum of arguments whose binomial(·,2) values sum to n. */
seq_t a(idx_t n)
{
    /* Dynamically grow a memoisation table.  dp[i] = a(i). */
    static seq_t *dp   = NULL;      /* array of previously computed values */
    static idx_t  sz   = 0;          /* number of entries allocated */

    /* Allocate or extend storage if needed.  All new entries are
       computed on the fly; old ones remain untouched.               */
    if (n + 1 > sz)
    {
        seq_t *new_dp = (seq_t *)realloc(dp, (size_t)(n + 1) * sizeof(seq_t));
        if (!new_dp) abort();
        dp = new_dp;

        /* First element (a(0)) is always 0. */
        if (sz == 0) dp[0] = (seq_t)0;
        size_t start = sz;   /* first index not yet initialised */

        for (size_t i = start; i <= n; ++i)
        {
            /* For i = 0 we already set dp[0] above. */
            if (i == 0) continue;

            /* Find the minimal sum for this i by trying all triangular
               coin values T_k = k*(k-1)/2 (k ≥ 2). */
            bool first = true;
            seq_t best  = 0;     /* value set when we see first candidate */

            for (size_t k = 2; ; ++k)
            {
                size_t tri = k * (k - 1) / 2;
                if (tri > i) break;      /* no larger k needed */

                seq_t cand = dp[i - tri] + (seq_t)k;
                if (first || cand < best)
                {
                    best  = cand;
                    first = false;
                }
            }

            dp[i] = best;
        }

        sz = n + 1;
    }

    return dp[n];
}
