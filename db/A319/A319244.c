/*
 * A319244  Expansion of Product_{k>0} (1 - x^(k^3))/(1 + x^(k^3)).
 * https://oeis.org/A319244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k>0} (1- x^(k^3))/(1+ x^(k^3))
 *            The implementation expands the product by iteratively convolving
 *            the generating function with each term (1-2t+2t^2-2t^3+…) where
 *            t = x^(k^3). Complexity is O(n^2) but for n ≤ 10 000 only about
 *            6×10^7 elementary operations, well within...
 * Range:     i8 n <= 76, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are provided by the build system.  Do not define them. */
seq_t a(idx_t n)
{
    /* Allocate DP arrays.  n ≤ 10 000 (typical), memory is modest. */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *tmp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp || !tmp)
    {
        if (dp) free(dp);
        if (tmp) free(tmp);
        /* memory error – return 0 */
        return (seq_t)0;
    }

    dp[0] = (seq_t)1;        /* a(0) = 1 */

    /* Iterate over cubes k^3 up to n */
    for (idx_t k = 1; ; ++k)
    {
        seq_t cube = ((seq_t)k) * k * k;  /* k^3, fits in seq_t */
        if (cube > n)
            break;

        /* Convert to idx_t for indexing */
        idx_t cubeIdx = (idx_t)cube;

        /* Start new array with existing coefficients (m = 0 term) */
        for (idx_t i = 0; i <= n; ++i)
            tmp[i] = dp[i];

        /* Add contributions of m ≥ 1 copies of this cube */
        for (idx_t m = 1; ; ++m)
        {
            idx_t m_cube = m * cubeIdx;
            if (m_cube > n)
                break;

            const seq_t double_val = (seq_t)2;
            if (m & 1) /* odd m → subtract 2*dp[...] */
            {
                for (idx_t s = m_cube; s <= n; ++s)
                {
                    seq_t val = dp[s - m_cube];
                    tmp[s] -= val + val;   /* -2 * val */
                }
            }
            else      /* even m → add 2*dp[...] */
            {
                for (idx_t s = m_cube; s <= n; ++s)
                {
                    seq_t val = dp[s - m_cube];
                    tmp[s] += val + val;   /* +2 * val */
                }
            }
        }

        /* Swap dp and tmp for the next iteration */
        seq_t *swap = dp;
        dp = tmp;
        tmp = swap;
    }

    /* Result is in dp[n] */
    seq_t result = dp[n];

    /* Clean up */
    free(dp);
    free(tmp);

    return result;
}
