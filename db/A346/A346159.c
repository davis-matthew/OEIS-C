/*
 * A346159  Number of n-dimensional representations of the group SU(3).
 * https://oeis.org/A346159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of partitions of n into parts j*k*(j+k)/2
 *            with j,k ≥ 1. The generating function is ∏_{j,k≥1} 1/(1 -
 *            x^{j*k*(j+k)/2})
 * Range:     u8 n <= 21, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate temporary array for standard coin‑change DP.  */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (dp == NULL)
        return (seq_t)0;   /* allocation failed – return 0 */

    /* Initialise dp[0] = 1, the rest 0.  */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* For every admissible pair (j,k), treat the weight w =
       j*k*(j+k)/2 as a coin type that can be used any number of times. */
    for (idx_t j = 1; ; ++j)
    {
        /* Smallest weight for this j occurs at k = 1.  */
        idx_t w_min = j * (j + 1) / 2;
        if (w_min > n)   /* no more weights possible */
            break;

        for (idx_t k = 1; ; ++k)
        {
            /* Compute w = j*k*(j+k)/2 using 128‑bit to avoid overflow.      */
            __int128 prod = (__int128)j * k * (j + k);   /* j*k*(j+k)   */
            idx_t w_idx = (idx_t)(prod / 2);            /* weigh cast <= n */

            if (w_idx > n)
                break;

            /* Unlimited coin change update for this weight. */
            for (idx_t i = w_idx; i <= n; ++i)
                dp[i] += dp[i - w_idx];
        }
    }

    /* Retrieve the answer, free memory, and return.  */
    seq_t result = dp[n];
    free(dp);
    return result;
}
