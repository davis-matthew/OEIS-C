/*
 * A261636  Expansion of Product_{k>=0} 1/(1-x^(4*k+1))^4.
 * https://oeis.org/A261636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A261636: coefficients of ∏_{k≥0} 1/(1-x^(4k+1))^4 .
   The routine uses dynamic programming over the part sizes 1,5,9,...,
   following the definition
        a(n) = ∑_{t≥0} C(t+3,3) · a_{prev}(n – t·m)
   for each part size m=4k+1.  It works for any seq_t (integer type or
   arbitrary‑precision class) and for any idx_t.  It does not use any
   external helper functions.  It is fully compatible with both C11
   and C++17. */
#include <stddef.h>
#ifdef __cplusplus
#  include <cstdlib>
#else
#  include <stdlib.h>
#endif

/* Binomial coefficient (t+3 choose 3) as seq_t. */
static seq_t binom3(idx_t t)
{
    seq_t v = (seq_t)(t + 1);
    v *= (t + 2);
    v *= (t + 3);
    v /= (seq_t)6;
    return v;
}

seq_t a(idx_t n)
{
    /* Handle the offset: a(0) = 1 */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Allocate two working arrays */
#ifdef __cplusplus
    seq_t *dp  = new seq_t[(size_t)(n + 1)];
    seq_t *ndp = new seq_t[(size_t)(n + 1)];
#else
    seq_t *dp  = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *ndp = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
#endif
    if (!dp || !ndp)
        return (seq_t)0;  /* allocation failure */

    /* Initialise: a(0)=1, others 0 */
    dp[0] = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* Process each part size m = 1,5,9,... up to n */
    for (idx_t m = (idx_t)1; m <= n; m += (idx_t)4)
    {
        /* Zero the new array */
        for (idx_t i = (idx_t)0; i <= n; ++i)
            ndp[i] = (seq_t)0;

        /* Pre‑compute binomial weights for this part size */
        idx_t maxT = n / m;
#ifdef __cplusplus
        seq_t *bins = new seq_t[(size_t)(maxT + 1)];
#else
        seq_t *bins = (seq_t*)malloc((size_t)(maxT + 1) * sizeof(seq_t));
#endif
        if (!bins)
            break; /* allocation failure – abort early */

        for (idx_t t = (idx_t)0; t <= maxT; ++t)
            bins[t] = binom3(t);

        /* Dynamic‑programming convolution */
        for (idx_t j = (idx_t)0; j <= n; ++j)
        {
            seq_t vdp = dp[j];
            if (vdp == (seq_t)0)
                continue;
            idx_t maxT2 = (n - j) / m;
            for (idx_t t = (idx_t)0; t <= maxT2; ++t)
            {
                ndp[j + t * m] += vdp * bins[t];
            }
        }

        /* Clean up binomial array */
#ifdef __cplusplus
        delete[] bins;
#else
        free(bins);
#endif

        /* Swap dp and ndp pointers for next iteration */
        seq_t *tmp = dp;
        dp = ndp;
        ndp = tmp;
    }

    seq_t result = dp[n];

    /* Free allocated memory */
#ifdef __cplusplus
    delete[] dp;
    delete[] ndp;
#else
    free(dp);
    free(ndp);
#endif

    return result;
}
