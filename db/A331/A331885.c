/*
 * A331885  Number of partitions of n into parts having a common factor > 1
 *          with n.
 * https://oeis.org/A331885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts having a common
 *            factor >1 with n
 * Range:     u8 n <= 29, u16 n <= 71, u32 n <= 197, u64 n <= 569,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;  /* the empty partition */

    /* allocate DP array of size n+1 */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;  /* allocation failure; caller must handle */

    dp[0] = (seq_t)1;
    for (idx_t j = 1; j <= n; ++j) dp[j] = (seq_t)0;

    const seq_t one = (seq_t)1;

    for (idx_t part = 2; part <= n; ++part) {
        if (gcd((seq_t)n, (seq_t)part) > one) {
            for (idx_t sum = part; sum <= n; ++sum) {
                dp[sum] += dp[sum - part];
            }
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
