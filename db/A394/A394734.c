/*
 * A394734  a(n) is the number of subsets of proper divisors of A023196(n)
 *          that add to A023196(n).
 * https://oeis.org/A394734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of the proper divisors of k that sum to
 *            k, where k = A023196(n) (nondeficient numbers, sigma(k) >= 2k);
 *            computed by a sigma sieve plus subset-count DP
 * Range:     u8 n <= 29, u16 n <= 176, u32 n <= 623, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 90 terms through n = 972 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint64_t *nd_list = NULL;
static size_t nd_cnt = 0;
static uint64_t nd_limit = 0;

static void nd_build(uint64_t L) {
    uint64_t *sg = (uint64_t *)calloc((size_t)L + 1, sizeof(uint64_t));
    if (!sg) return;
    for (uint64_t i = 1; i <= L; i++)
        for (uint64_t j = i; j <= L; j += i)
            sg[j] += i;
    free(nd_list);
    nd_list = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)L + 1));
    nd_cnt = 0;
    for (uint64_t k = 1; k <= L; k++)
        if (sg[k] >= 2 * k) nd_list[nd_cnt++] = k;
    nd_limit = L;
    free(sg);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    while (nd_cnt < (size_t)n) {
        uint64_t L = nd_limit ? nd_limit * 2 : 8192;
        nd_build(L);
        if (nd_limit != L) return 0;
    }
    uint64_t k = nd_list[(size_t)n - 1];
    seq_t *dp = mem_new(seq_t, (size_t)k + 1);
    dp[0] = 1;
    for (uint64_t d = 1; d <= k / 2; d++) {
        if (k % d != 0) continue;
        for (uint64_t s = k; s >= d; s--)
            dp[(size_t)s] += dp[(size_t)(s - d)];
    }
    seq_t r = dp[(size_t)k];
    mem_free(dp);
    return r;
}
