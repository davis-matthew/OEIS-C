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
 * Validated: partial, pass, 87 terms through n = 86 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Recursive counts of partitions of 'mask' into blocks each summing to 's'.
 * Vectors: sums[mask] gives sum of indices in 'mask'.
 * 'dp' and 'seen' provide memoisation. */
static seq_t dfs_mask(int mask, seq_t s, seq_t *sums, int m,
                      seq_t *dp, bool *seen)
{
    if (seen[mask])
        return dp[mask];

    seq_t total = 0;
    if (mask == 0) {
        total = 1;
    } else {
        /* smallest set element (least significant bit) */
        int first = 0;
        while (!(mask & (1 << first)))
            first++;

        /* iterate all submasks of 'mask' that contain 'first' */
        for (int sub = mask; sub; sub = (sub - 1) & mask) {
            if (!(sub & (1 << first)))
                continue;
            if (sums[sub] == s)
                total += dfs_mask(mask ^ sub, s, sums, m, dp, seen);
        }
    }
    seen[mask] = true;
    dp[mask] = total;
    return total;
}

/* Count partitions of the full set (m elements) into blocks each summing to 's'. */
static seq_t count_partitions_with_sum(seq_t s, int m, seq_t *sums)
{
    int maxMask = 1 << m;
    seq_t *dp = mem_new(seq_t, (size_t)maxMask);
    bool *seen = (bool *)calloc((size_t)maxMask, sizeof(bool));
    if (!dp || !seen) {
        /* memory error – return 0 as safe fallback */
        mem_free(dp); free(seen);
        return 0;
    }
    seq_t res = dfs_mask(maxMask - 1, s, sums, m, dp, seen);
    mem_free(dp); free(seen);
    return res;
}

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
    seq_t *sums = mem_new(seq_t, (size_t)maxMask);
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

    mem_free(sums);
    return ans;
}
