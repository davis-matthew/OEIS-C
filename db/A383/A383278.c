/*
 * A383278  The number of integers k such that A034444(k) * k <= n.
 * https://oeis.org/A383278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{k >= 1 : 2^omega(k) * k <= n}, computed by sieving
 *            omega(k) up to n
 * Range:     u8 n <= 957, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9220 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t N = (uint64_t)n;
    uint8_t *w = (uint8_t *)calloc((size_t)N + 1, 1);
    if (!w) return (seq_t)0;
    for (uint64_t p = 2; p <= N; ++p) {
        if (w[p] == 0) {
            for (uint64_t m = p; m <= N; m += p)
                ++w[m];
        }
    }
    seq_t count = 0;
    for (uint64_t k = 1; k <= N; ++k) {
        /* 2^omega(k) * k <= n  <=>  k <= n >> omega(k) */
        if (k <= (N >> w[k]))
            ++count;
    }
    free(w);
    return count;
}
