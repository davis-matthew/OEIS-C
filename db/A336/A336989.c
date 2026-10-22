/*
 * A336989  Expansion of Product_{k>=1} (1 + x^k / (1 - k*x)).
 * https://oeis.org/A336989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A336989
   Formula: a(n) = coefficient of x^n in ∏_{k≥1} (1 + x^k/(1 - kx))
             Product expanded up to degree n. */

/* Include required standard header */
#include <stdlib.h>

/* Computes a(n) for the sequence A336989 */
seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 */
    if (n == 0)
        return (seq_t)1;

    /* Work with ordinary C size_t for array indices */
    size_t N = (size_t)n;

    /* Allocate arrays for dynamic programming */
    seq_t *dp   = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *ndp  = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *tmp  = (seq_t *)calloc(N + 1, sizeof(seq_t));

    /* Base case: product of empty set has coefficient 1 at degree 0 */
    dp[0] = (seq_t)1;

    /* Iterate over factors k = 1..N
       dp[i] holds the coefficient of x^i after processing factors < k */
    for (size_t k = 1; k <= N; ++k)
    {
        /* Compute convolution of dp with the geometric series
           tmp[i] = Σ_{m=0..i} k^m * dp[i - m] */
        tmp[0] = dp[0];
        for (size_t i = 1; i <= N; ++i)
        {
            tmp[i] = dp[i] + ((seq_t)k * tmp[i - 1]);
        }

        /* Update dp for current k:
           newdp[i] = dp[i] + (i >= k ? tmp[i - k] : 0) */
        for (size_t i = 0; i <= N; ++i)
        {
            if (i >= k)
                ndp[i] = dp[i] + tmp[i - k];
            else
                ndp[i] = dp[i];
        }

        /* Prepare for next iteration: swap dp and ndp pointers */
        seq_t *tmp_ptr = dp;
        dp = ndp;
        ndp = tmp_ptr;
    }

    /* dp[N] now contains a(n) */
    seq_t result = dp[N];

    /* Free allocated memory */
    free(dp);
    free(ndp);
    free(tmp);

    return result;
}
