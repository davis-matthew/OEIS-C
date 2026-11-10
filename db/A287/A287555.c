/*
 * A287555  Positions of 3 in A053839.
 * https://oeis.org/A287555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(n-1) + ((3 - s(n-1)) mod 4) + 1, where s(k) = sum of
 *            base-4 digits of k (positions of 3 in A053839, counted from 1)
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9563 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned A053839_a(idx_t n)
{
    unsigned sum = 0;
    idx_t x = n;
    while (x != 0) {
        sum += (unsigned)(x & 3);
        x >>= 2;
    }
    return sum & 3u;
}

seq_t a(idx_t n)
{
    idx_t k = n - 1;
    unsigned d = (3u + 4u - A053839_a(k)) & 3u;
    return (seq_t)k * 4 + (seq_t)d + 1;
}
