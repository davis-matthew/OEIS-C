/*
 * A247599  Number of ways of writing n as a sum: n = 2^0*k(0)^3 + 2^1*k(1)^3
 *          + 2^2*k(2)^3 + ... where the k's are nonnegative integers.
 * https://oeis.org/A247599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 766, u16 n <= 8383, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A247599: number of representations of n as
   n = 2^0*k0^3 + 2^1*k1^3 + 2^2*k2^3 + ...  (ki ≥ 0).  */
/* This implementation uses a DP over the powers of two that
   behaves in O(n * n^(1/3)) time and uses O(n) memory.  */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/int_cube_root.h"

#ifdef __cplusplus
#  include <cstdint>
#  include <cstdlib>
#endif

/* seq_t and idx_t are supplied by the build environment. */

seq_t a(idx_t n)
{
    /* Allocate two arrays of seq_t of length n+1. */
#if defined(__cplusplus)
    seq_t *dp  = new seq_t[n + 1]();   /* zero‑initialized */
    seq_t *newdp = new seq_t[n + 1]();
#else
    seq_t *dp  = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    seq_t *newdp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp || !newdp) {
        /* Allocation failed – return 0 as a safe fallback. */
        free(dp); free(newdp);
        return (seq_t)0;
    }
    /* Explicitly set to zero (safe for integer types and triggers
       assignment operator for class types). */
    for (size_t i = 0; i <= (size_t)n; ++i) {
        dp[i]  = (seq_t)0;
        newdp[i] = (seq_t)0;
    }
#endif

    /* Base case: one way to make 0. */
    dp[0] = (seq_t)1;

    /* Iterate over powers of two: 1, 2, 4, ... <= n. */
    for (idx_t pow = 1; pow <= n; pow <<= 1) {
        /* Copy current dp into newdp (enabling the use of coin value 0). */
        for (size_t i = 0; i <= (size_t)n; ++i)
            newdp[i] = dp[i];

        /* Maximum cube coefficient that can be multiplied by pow without exceeding n. */
        idx_t rem = n / pow;
        idx_t tmax = int_cube_root(rem);

        /* For every existing sum add contributions of new cubes. */
        for (size_t sum = 0; sum <= (size_t)n; ++sum) {
            if (dp[sum] == (seq_t)0)
                continue;
            for (idx_t t = 1; t <= tmax; ++t) {
                /* weight = pow * t^3 */
                idx_t t3 = t * t * t;
                seq_t weight = (seq_t)pow * (seq_t)t3;
                idx_t newSum = sum + (idx_t)weight;
                if (newSum > n)
                    break;
                newdp[newSum] += dp[sum];
            }
        }

        /* Swap dp and newdp for next iteration. */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    seq_t result = dp[n];

    /* Free allocated memory. */
#if defined(__cplusplus)
    delete[] dp;
    delete[] newdp;
#else
    free(dp);
    free(newdp);
#endif

    return result;
}
