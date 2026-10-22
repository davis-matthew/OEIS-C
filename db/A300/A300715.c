/*
 * A300715  Number of compositions (ordered partitions) of n into squares that
 *          do not divide n.
 * https://oeis.org/A300715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered compositions of n into squares k^2 with
 *            k^2 not dividing n. Computed with a simple dynamic‑programming
 *            routine over the allowed square parts
 * Range:     u8 n <= 54, u16 n <= 94, u32 n <= 173, u64 n <= 333,
 *            u128 n <= 648, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute floor(sqrt(n)) */
    idx_t max_k = 0;
    while ((max_k + 1) * (max_k + 1) <= n) {
        ++max_k;
    }

    /* Allocate DP array of size n+1 */
    size_t len = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp) return (seq_t)0; /* allocation failure */

    for (size_t i = 0; i < len; ++i) {
        dp[i] = 0;
    }
    dp[0] = 1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t acc = 0;
        for (idx_t k = 1; k <= max_k; ++k) {
            idx_t s = k * k;
            if (s > i) break;
            if (n % s == 0) continue;   /* square divides n → not allowed */
            acc += dp[(size_t)(i - s)];
        }
        dp[(size_t)i] = acc;
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
}
