/*
 * A308869  Sum of the fifth largest parts in the partitions of n into 6
 *          parts.
 * https://oeis.org/A308869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Implements OEIS A308869: sum of the fifth largest parts in the partitions
 * of n into 6 parts.
 *
 * The function uses dynamic programming on partitions into exactly `k`
 * parts, computing for each (k,s) the number of partitions `dp[k][s]`,
 * the sum of their smallest parts `small[k][s]`,
 * and the sum of their second smallest parts `second[k][s]`.
 *
 * Recurrences (for s ≥ k ≥ 1):
 *   dp[k][s]     = dp[k][s-k] + dp[k-1][s-1]
 *   small[k][s]  = small[k][s-k] + dp[k][s-k] + dp[k-1][s-1]
 *   second[k][s] = second[k][s-k] + dp[k][s-k] + small[k-1][s-1]
 *
 * The desired value a(n) is second[6][n].  All arrays are sized to
 * indices 0..n, and 0 entries are naturally zero.  The implementation
 * supports both C11 and C++17, allocating memory with `calloc` for C
 * and with `std::vector` for C++ to guarantee proper construction.
 *
 * Complexity: O(n) time and O(n) space (for fixed k=6).
 */
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    const idx_t K = 6;
    const idx_t N = n;

    /* total array size: (K+1) * (N+1) */
    const size_t total = (size_t)(K + 1) * (N + 1);

#ifdef __cplusplus
    std::vector<seq_t> dp_vec(total);
    std::vector<seq_t> small_vec(total);
    std::vector<seq_t> second_vec(total);
#else
    seq_t *dp_vec    = calloc(total, sizeof(seq_t));
    seq_t *small_vec = calloc(total, sizeof(seq_t));
    seq_t *second_vec = calloc(total, sizeof(seq_t));
#endif

    /* helper to compute array index (k, s) */
    const size_t stride = N + 1;

    for (idx_t k = 1; k <= K; ++k) {
        size_t base_k = k * stride;
        size_t base_km1 = (k - 1) * stride;

        if (k == 1) {
            for (idx_t s = 1; s <= N; ++s) {
                dp_vec[base_k + s] = (seq_t)1;
                small_vec[base_k + s] = (seq_t)s;     /* smallest part is the part itself */
                second_vec[base_k + s] = (seq_t)0;    /* no second part */
            }
        } else {
            for (idx_t s = k; s <= N; ++s) {
                /* components of the recurrence */
                seq_t part_shift  = dp_vec[base_k + (s - k)];   /* dp[k][s-k] */
                seq_t small_shift = small_vec[base_k + (s - k)];/* small[k][s-k] */
                seq_t second_shift= second_vec[base_k + (s - k)];/* second[k][s-k] */

                seq_t part_1  = dp_vec[base_km1 + (s - 1)];   /* dp[k-1][s-1] */
                seq_t small_1 = small_vec[base_km1 + (s - 1)];/* small[k-1][s-1] */

                dp_vec[base_k + s]     = part_shift + part_1;
                small_vec[base_k + s]  = small_shift + part_shift + part_1;
                second_vec[base_k + s] = second_shift + part_shift + small_1;
            }
        }
    }

    seq_t result = second_vec[K * stride + N];

#ifndef __cplusplus
    free(dp_vec);
    free(small_vec);
    free(second_vec);
#endif

    return result;
}
