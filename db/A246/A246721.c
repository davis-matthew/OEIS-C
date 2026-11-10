/*
 * A246721  Number of partitions of n into parts of the n-th list of distinct
 *          parts in the order given by A246688.
 * https://oeis.org/A246721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A246720(n,n): number of partitions of n into the parts of
 *            the n-th distinct-part list of A246688 (lists ordered by sum,
 *            then lexicographically); computed by enumerating lists, then
 *            coin-change DP
 * Range:     u8 n <= 32, u16 n <= 109, u32 n <= 1068, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 89 terms through n = 4611 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static idx_t A246721_cnt;
static idx_t A246721_target;
static idx_t A246721_cur[4096];
static idx_t A246721_res[4096];
static size_t A246721_reslen;

static bool A246721_gen(idx_t remaining, idx_t min_part, size_t depth)
{
    if (remaining == 0) {
        if (A246721_cnt == A246721_target) {
            A246721_reslen = depth;
            for (size_t i = 0; i < depth; ++i) A246721_res[i] = A246721_cur[i];
            return true;
        }
        ++A246721_cnt;
        return false;
    }
    for (idx_t part = min_part; part <= remaining; ++part) {
        if (part != remaining && remaining - part < part + 1) continue;
        A246721_cur[depth] = part;
        if (A246721_gen(remaining - part, part + 1, depth + 1)) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    A246721_cnt = 1; /* index 0 is the empty list */
    A246721_target = n;
    A246721_reslen = 0;
    for (idx_t m = 1;; ++m) {
        if (A246721_gen(m, 1, 0)) break;
    }
    seq_t *dp = mem_new(seq_t, (size_t)n + 1);
    dp[0] = 1;
    for (size_t i = 0; i < A246721_reslen; ++i) {
        idx_t p = A246721_res[i];
        if (p > n) continue;
        for (idx_t j = p; j <= n; ++j) dp[j] += dp[j - p];
    }
    seq_t r = dp[n];
    mem_free(dp);
    return r;
}
