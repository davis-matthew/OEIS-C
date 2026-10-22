/*
 * A342331  Number of compositions of n where each part after the first is
 *          either twice or half the prior part.
 * https://oeis.org/A342331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 62, u32 n <= 128, u64 n <= 254,
 *            u128 n <= 512, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Σ_{d|n, d odd} W(n/d)
 * where W(m) counts the number of compositions of m into powers of two
 * with the property that consecutive parts differ by a factor of 2 or 1/2.
 * W(m) is computed via a dynamic programming over sums and exponents.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special case */
    if (n == 0)
        return (seq_t)1;

    /* Number of exponents that can occur for sums up to n
       expCount = floor(log2(n)) + 1 (exponents 0 .. expCount-1) */
    int expCount = 1;                 /* at least exponent 0 */
    idx_t tmp = n;
    while ((tmp >>= 1) != 0)
        ++expCount;

    /* Allocate DP array: dp[s * expCount + e] holds number of
       ways to compose sum s ending with exponent e. */
#ifdef __cplusplus
    seq_t *dp = new seq_t[(size_t)(n + 1) * expCount];
#else
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1) * expCount, sizeof(seq_t));
#endif
    if (!dp)
        return (seq_t)0;          /* allocation failure */

    /* Initialise starting compositions with a single part 2^e. */
    for (int e = 0; e < expCount; ++e) {
        idx_t pow = ((idx_t)1) << e;
        if (pow > n)
            break;
        dp[pow * expCount + e] = 1;
    }

    /* Transitions: for each existing composition, extend it by one more part. */
    for (idx_t s = 1; s <= n; ++s) {
        for (int e = 0; e < expCount; ++e) {
            seq_t val = dp[s * expCount + e];
            if (val == 0)
                continue;

            /* Append part 2^(e+1) if it stays within bounds. */
            if (e + 1 < expCount) {
                idx_t step = ((idx_t)1) << (e + 1);
                idx_t ns = s + step;
                if (ns <= n)
                    dp[ns * expCount + (e + 1)] += val;
            }

            /* Append part 2^(e-1) (only if e > 0). */
            if (e > 0) {
                idx_t step = ((idx_t)1) << (e - 1);
                idx_t ns = s + step;
                if (ns <= n)
                    dp[ns * expCount + (e - 1)] += val;
            }
        }
    }

    /* Compute W[s] = number of compositions of s into powers of two. */
#ifdef __cplusplus
    seq_t *W = new seq_t[n + 1];
    for (size_t i = 0; i <= (size_t)n; ++i) W[i] = 0;
#else
    seq_t *W = (seq_t *)calloc(n + 1, sizeof(seq_t));
#endif
    if (!W) {
#ifdef __cplusplus
        delete[] dp;
#else
        free(dp);
#endif
        return (seq_t)0;
    }

    for (idx_t s = 0; s <= n; ++s) {
        seq_t sum = 0;
        for (int e = 0; e < expCount; ++e)
            sum += dp[s * expCount + e];
        W[s] = sum;
    }

    /* The answer is the sum of W[n/d] over all odd divisors d of n. */
    seq_t ans = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;
        if (d & 1)
            ans += W[n / d];
        idx_t e = n / d;
        if (e != d && (e & 1))
            ans += W[n / e];
    }

    /* Free temporary arrays. */
#ifdef __cplusplus
    delete[] dp;
    delete[] W;
#else
    free(dp);
    free(W);
#endif

    return ans;
}
