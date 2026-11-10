/*
 * A279340  First differences of A055938.
 * https://oeis.org/A279340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*A079559(n) + 1, where A079559(n) = number of partitions
 *            of n into distinct parts of the form 2^j-1
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 136 terms through n = 7735 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t mers[70];
    size_t count = 0;
    for (unsigned j = 1; j < 64; ++j) {
        idx_t val = (((idx_t)1) << j) - 1;
        if (val > n) break;
        mers[count++] = val;
    }

    seq_t *dp = mem_new(seq_t, (size_t)n + 1);
    if (!dp) return 0;
    dp[0] = 1;

    for (size_t mi = 0; mi < count; ++mi) {
        idx_t m = mers[mi];
        for (idx_t rest = n; rest >= m; --rest) {
            dp[rest] += dp[rest - m];
        }
    }

    seq_t result = dp[n];
    mem_free(dp);
    return result * 2 + 1;
}
