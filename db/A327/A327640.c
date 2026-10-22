/*
 * A327640  Number of partitions of n into divisors d of n such that n/d is
 *          odd.
 * https://oeis.org/A327640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into divisors d of n such that
 *            n / d is odd. Computed by ordinary coin–change DP over the set {
 *            d | d divides n and (n/d) is odd }
 * Range:     u8 n <= 44, u16 n <= 224, u32 n <= 944, u64 n <= 3464,
 *            u128 n <= 9971, big any n
 * Helpers:   part_contains
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc, free */
#include "../utils/part_contains.h"

static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid != 0 && mid > n / mid) {
            /* mid*mid > n */
            high = mid - 1;
        } else {
            /* mid*mid <= n */
            if (mid * mid == n) return mid;
            low = mid + 1;
        }
    }
    return high;          /* floor sqrt */
}

seq_t a(idx_t n)
{
    /* Special case: n == 0 */
    if (n == 0) return (seq_t)1;

    /* Build list of admissible parts */
    const idx_t MAX_PARTS = 2048;
    idx_t parts[MAX_PARTS];
    size_t m = 0;

    idx_t r = isqrt_u64(n);
    for (idx_t d = 1; d <= r; ++d) {
        if (n % d == 0) {
            idx_t q = n / d;
            if (q & 1ULL) {
                /* d is allowed when quotient is odd */
                if (!part_contains(parts, m, d)) {
                    if (m < MAX_PARTS) parts[m++] = d;
                }
            }
            if (d & 1ULL) {
                /* n/d is allowed when d is odd */
                idx_t d2 = q;
                if (!part_contains(parts, m, d2)) {
                    if (m < MAX_PARTS) parts[m++] = d2;
                }
            }
        }
    }

    /* DP for partition function */
    seq_t *dp;
#ifdef __cplusplus
    dp = new seq_t[n + 1];
#else
    dp = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (dp == NULL) return 0;   /* Out‑of‑memory fallback */
#endif
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

    for (size_t k = 0; k < m; ++k) {
        idx_t p = parts[k];
        for (idx_t j = p; j <= n; ++j) {
            dp[j] += dp[j - p];
        }
    }

    seq_t result = dp[n];
#ifdef __cplusplus
    delete [] dp;
#else
    free(dp);
#endif
    return result;
}
