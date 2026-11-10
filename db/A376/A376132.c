/*
 * A376132  First differences of A376131.
 * https://oeis.org/A376132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000295(A376116(n+1) - A376116(n) + 1), where A000295(k)
 *            = 2^k - k - 1
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Validated: partial, pass, 88 terms through n = 88 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A376132_root(uint64_t n)
{
    uint64_t val = (n << 1) + 1ULL;
    int m = 63 - __builtin_clzll(val);
    uint64_t sum = 0;
    for (int j = 1; j < m; ++j) {
        uint64_t bit = (val >> j) & 1ULL;
        sum += ((1ULL << j) - 1) * (bit + 1);
    }
    return sum;
}

seq_t a(idx_t n)
{
    uint64_t d = A376132_root((uint64_t)n + 1) - A376132_root((uint64_t)n);
    uint64_t k = d + 1;
    seq_t r = 0;
    for (uint64_t i = 1; i <= k; i++) {
        r = r * 2 + (seq_t)(i - 1);
    }
    return r;
}
