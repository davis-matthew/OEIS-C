/*
 * A371735  Maximal length of a set partition of the binary indices of n into
 *          blocks all having the same sum.
 * https://oeis.org/A371735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximal number of blocks in a partition of the binary
 *            indices of n into blocks all having the same sum. The algorithm
 *            enumerates all possible block counts (divisors of the total sum)
 *            and checks feasibility using a backtracking recursion that
 *            explores subsets summing to the target
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   rec_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/rec_part.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* a(0) = 0 */

    /* collect indices of set bits (positions start at 1) */
    int pos[64];
    int m = 0;
    idx_t tmp = n;
    int bit = 1;
    while (tmp) {
        if (tmp & 1ULL) pos[m++] = bit;
        bit++;
        tmp >>= 1;
    }

    /* precompute subset sums */
    int subset_size = 1 << m;
    int *subset_sum = (int *)malloc(subset_size * sizeof(int));
    subset_sum[0] = 0;
    for (int mask = 1; mask < subset_size; ++mask) {
        int b = __builtin_ctz(mask);
        subset_sum[mask] = subset_sum[mask ^ (1 << b)] + pos[b];
    }
    int total_sum = subset_sum[subset_size - 1];

    /* maximum position (needed for a quick feasibility test) */
    int max_pos = pos[0];
    for (int i = 1; i < m; ++i) if (pos[i] > max_pos) max_pos = pos[i];

    /* try block counts from maximum possible (m) down to 1 */
    for (int k = m; k >= 1; --k) {
        if (total_sum % k != 0) continue;
        int target = total_sum / k;
        if (target < max_pos) continue;   /* impossible to fit the largest element */

        /* memoization table for this target */
        int stride = m + 1;
        int8_t *memo = (int8_t *)malloc(subset_size * stride * sizeof(int8_t));
        for (int i = 0; i < subset_size * stride; ++i) memo[i] = -1;

        if (rec_part(subset_size - 1, k, target, subset_sum, memo, stride)) {
            free(subset_sum);
            free(memo);
            return (seq_t)k;
        }
        free(memo);
    }

    /* should not reach here; at least k=1 works */
    free(subset_sum);
    return 1;
}
