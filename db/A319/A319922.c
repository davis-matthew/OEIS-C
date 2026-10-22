/*
 * A319922  Number of ways of writing n as a sum of exactly 10 powers of 2.
 * https://oeis.org/A319922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = coefficient of x^n y^10 in 1/∏_{j≥0} (1-y x^(2^j))
 * Range:     u8 n <= 9659, u16 n <= 9659, u32 n <= 9659, u64 n <= 9659,
 *            u128 n <= 9659, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function counts the number of unordered representations of n
   as a sum of exactly 10 powers of 2.  The algorithm uses a
   standard “coin change” dynamic‑programming approach with 11 part
   counts (0..10).  Complexity: O(n * log n * 10).  The routine works
   for any seq_t and idx_t defined by the build. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    const int K = 10;                 /* number of parts required */
    const size_t stride = K + 1;      /* entries per sum value */
    const size_t total  = ((size_t)n + 1) * stride;

    /* Allocate DP table: dp[s * stride + k] = ways to form sum s using k parts */
#ifdef __cplusplus
    seq_t *dp = new seq_t[total]();   /* value‑initialise to zero */
#else
    seq_t *dp = (seq_t *)calloc(total, sizeof(seq_t));
#endif
    if (!dp)                     /* allocation failure → 0 */
        return 0;

    dp[0 * stride + 0] = 1;      /* base case: empty sum */

    /* Process each power of two up to n */
    for (idx_t p = 1; p <= n; p <<= 1) {
        for (idx_t s = p; s <= n; ++s) {
            size_t idx_base = (size_t)s * stride;
            size_t idx_prev = (size_t)(s - p) * stride;
            for (int k = 1; k <= K; ++k) {
                dp[idx_base + k] += dp[idx_prev + (k - 1)];
            }
        }
    }

    seq_t result = dp[(size_t)n * stride + K];
#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif
    return result;
}
