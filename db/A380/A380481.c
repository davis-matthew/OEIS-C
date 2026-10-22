/*
 * A380481  Number of partitions of n into distinct parts less than n and not
 *          a multiple of 3.
 * https://oeis.org/A380481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of {k | 1 ≤ k ≤ n-1, k mod 3 ≠ 0} whose
 *            elements sum to n
 * Range:     u8 n <= 44, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>

/* Compute a(n) for all n ≥ 0 as defined for sequence A380481 */
seq_t a(idx_t n)
{
    /* Special case: a(0) = 1 */
    if (n == (idx_t)0)
        return (seq_t)1;

#ifdef __cplusplus
    /* Use std::vector for C++ builds to construct objects safely */
    std::vector<seq_t> dp((size_t)n + 1, (seq_t)0);
    dp[0] = (seq_t)1;
    for (idx_t p = (idx_t)1; p < n; ++p) {
        if (p % (idx_t)3 == (idx_t)0) continue;          /* skip multiples of 3 */
        for (idx_t j = n; j >= p; --j) {
            dp[(size_t)j] += dp[(size_t)(j - p)];
        }
    }
    return dp[(size_t)n];
#else
    /* Use plain C allocation for builds where seq_t is a plain unsigned type */
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp) return (seq_t)0;  /* allocation failure; return 0 as a safe fallback */
    dp[0] = (seq_t)1;
    for (idx_t p = (idx_t)1; p < n; ++p) {
        if (p % (idx_t)3 == (idx_t)0) continue;          /* skip multiples of 3 */
        for (idx_t j = n; j >= p; --j) {
            dp[(size_t)j] += dp[(size_t)(j - p)];
        }
    }
    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
#endif
}
