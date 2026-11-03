/*
 * A341167  Number of partitions of n into 9 distinct nonzero decimal
 *          palindromes.
 * https://oeis.org/A341167
 *
 * Offset:    45    (a(45) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of partitions of n into 9 distinct nonzero decimal
 *            palindromes
 * Range:     u8 n <= 197, u16 n <= 601, u32 n <= 4722, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 92 terms through n = 9838 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A341167: the number of partitions of n into 9 distinct nonzero decimal palindromes.
 * dp[k][s] = the number of ways to write s as k distinct parts, by a 0/1
 * knapsack over the parts in increasing order, up to a limit that grows with n
 * (the original stopped at a fixed limit). Counts are sums only, so exact
 * modulo the width. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

#define K 9

static int is_part(uint64_t m)
{
    /* a nonzero decimal palindrome */
    uint64_t r = 0, x = m;
    while (x) { r = r * 10 + x % 10; x /= 10; }
    return m > 0 && r == m;
}

static seq_t *dp = NULL;                  /* dp[k * (L + 1) + s] */
static uint64_t L = 0;

static void build(uint64_t lim)
{
    mem_free(dp);
    dp = mem_new(seq_t, (size_t)(K + 1) * (lim + 1));
    if (!dp) abort();
    L = lim;
    dp[0] = (seq_t)1;
    for (uint64_t p = 1; p <= L; ++p) {
        if (!is_part(p)) continue;
        for (int k = K; k >= 1; --k)
            for (uint64_t s = L; s >= p; --s)
                dp[(size_t)k * (L + 1) + s] += dp[(size_t)(k - 1) * (L + 1) + s - p];
    }
}

seq_t a(idx_t n)
{
    if ((uint64_t)n > L || !dp) {
        uint64_t lim = L ? 2 * L : 4096;
        while (lim < (uint64_t)n) lim *= 2;
        build(lim);
    }
    return dp[(size_t)K * (L + 1) + (uint64_t)n];
}
