/*
 * A336137  Number of set partitions of the binary indices of n with equal
 *          block-sums.
 * https://oeis.org/A336137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of set partitions of the binary indices of n *
 *            with equal block-sums. Indices are positions (starting at 1) of
 *            * the 1-bits in the binary representation of n
 *            (least‑significant * bit has index 1). The function works for
 *            all n ≥ 0
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   count_partitions_with_sum, dfs_mask
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/count_partitions_with_sum.h"
#include "../utils/dfs_mask.h"

seq_t a(idx_t n)
{
    /* Gather binary indices (positions of set bits, LSB=1). */
    int pos[64];
    int m = 0;
    idx_t tmp = n;
    int idx = 1;
    while (tmp != 0) {
        if (tmp & 1) {
            pos[m++] = idx;
        }
        tmp >>= 1;
        idx++;
    }

    /* No bits set → empty set → exactly one partition (empty). */
    if (m == 0)
        return 1;

    /* Total sum of indices. */
    seq_t total = 0;
    for (int i = 0; i < m; ++i)
        total += (seq_t)pos[i];

    /* Precompute sums for all subsets. */
    int maxMask = 1 << m;
    seq_t *sums = (seq_t *)malloc((size_t)maxMask * sizeof(seq_t));
    if (!sums) return 0;
    sums[0] = 0;
    for (int mask = 1; mask < maxMask; ++mask) {
        /* least significant set bit */
        int lsb = __builtin_ctz((unsigned)mask);
        sums[mask] = sums[mask ^ (1 << lsb)] + (seq_t)pos[lsb];
    }

    /* Enumerate all k such that k divides total and k ≤ m. */
    seq_t ans = 0;
    uint64_t T = (uint64_t)total;   /* fits in 64‑bit for m ≤ 14 */
    for (uint64_t d = 1; d * d <= T; ++d) {
        if (T % d != 0)
            continue;
        uint64_t k1 = d;
        uint64_t k2 = T / d;

        if (k1 <= (uint64_t)m) {
            seq_t s = total / (seq_t)k1;
            ans += count_partitions_with_sum(s, m, sums);
        }
        if (k2 != k1 && k2 <= (uint64_t)m) {
            seq_t s = total / (seq_t)k2;
            ans += count_partitions_with_sum(s, m, sums);
        }
    }

    free(sums);
    return ans;
}
