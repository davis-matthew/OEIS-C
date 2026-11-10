/*
 * A283069  A283050(n) divided by the square of its least prime factor.
 * https://oeis.org/A283069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A283050(n) / A020639(A283050(n))^2; search: scan integers
 *            i >= 4 in order, keep those divisible by (least prime factor)^2,
 *            take the n-th, return i/p^2
 * Range:     u8 n <= 338, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 8863 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A283069_lpf(uint64_t m)
{
    if ((m & 1) == 0) return 2;
    for (uint64_t d = 3; d <= m / d; d += 2)
        if (m % d == 0) return d;
    return m;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t count = 0;
    for (uint64_t i = 4; ; ++i) {
        uint64_t p = A283069_lpf(i);
        uint64_t sq = p * p;
        if (i % sq == 0) {
            ++count;
            if (count == n) return (seq_t)(i / sq);
        }
    }
}
