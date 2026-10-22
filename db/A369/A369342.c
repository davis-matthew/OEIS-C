/*
 * A369342  Number of compositions (ordered partitions) of n into squares not
 *          greater than sqrt(n).
 * https://oeis.org/A369342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 36, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n>0, a(n)=∑_{k=1}^{⌊√√n⌋} a(n-k²) */
#include <stdlib.h>
#include <stdint.h>

/* Integer square‑root: floor(sqrt(n)) for idx_t. */
static idx_t _isqrt_u64(idx_t n)
{
    if (n == 0) return 0;
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    /* Base case for n = 0. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute the maximum allowed square root kmax = floor(sqrt(sqrt(n))). */
    idx_t kmax = _isqrt_u64(_isqrt_u64(n));
    if (kmax == 0) kmax = 1;  /* safety, should not happen for n>0 */

    /* Allocate DP array of size n+1. */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) {
        return 0;  /* out of memory; caller may handle */
    }

    /* Initialize dp[0]=1, others 0. */
    for (idx_t i = 0; i <= n; ++i) {
        dp[i] = (seq_t)0;
    }
    dp[0] = (seq_t)1;

    /* Fill DP using allowed squares. */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= kmax; ++k) {
            idx_t s = k * k;
            if (s > i) break;
            sum += dp[(size_t)(i - s)];
        }
        dp[i] = sum;
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
}
