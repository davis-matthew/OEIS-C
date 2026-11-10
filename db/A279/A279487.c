/*
 * A279487  Indices k such that A279485(k) = 0.
 * https://oeis.org/A279487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th index k with [x^k] Product_{j>=1} (1-x^(j^4)) = 0
 *            (A279485(k)=0); computed by sieve/DP over the product, limit
 *            doubled until enough terms
 * Range:     u8 n <= 248, u16 n <= 58759, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 83 terms through n = 9475 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *A279487_zeros = NULL;
static uint64_t A279487_count = 0;

static void A279487_build(uint64_t limit)
{
    int32_t *dp = (int32_t *)calloc((size_t)limit + 1, sizeof(int32_t));
    if (!dp) return;
    dp[0] = 1;
    for (uint64_t k = 1; ; ++k) {
        uint64_t v = k * k * k * k;
        if (v > limit) break;
        for (uint64_t i = limit; i >= v; --i)
            dp[i] -= dp[i - v];
    }
    uint64_t cnt = 0;
    for (uint64_t i = 1; i <= limit; ++i)
        if (dp[i] == 0) ++cnt;
    uint32_t *z = (uint32_t *)malloc((size_t)(cnt ? cnt : 1) * sizeof(uint32_t));
    if (!z) { free(dp); return; }
    uint64_t p = 0;
    for (uint64_t i = 1; i <= limit; ++i)
        if (dp[i] == 0) z[p++] = (uint32_t)i;
    free(dp);
    free(A279487_zeros);
    A279487_zeros = z;
    A279487_count = cnt;
}

seq_t a(idx_t n)
{
    static uint64_t limit = 0;
    if (n == 0) return (seq_t)0;
    while (A279487_count < (uint64_t)n && limit < (1ULL << 27)) {
        limit = limit ? limit * 2 : 32768;
        A279487_build(limit);
    }
    if ((uint64_t)n > A279487_count) return (seq_t)0;
    return (seq_t)A279487_zeros[n - 1];
}
