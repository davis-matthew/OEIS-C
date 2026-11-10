/*
 * A287552  Positions of 0 in A053839.
 * https://oeis.org/A287552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4n - 3 + ((-s(n-1)) mod 4), where s(j) = sum of base-4
 *            digits of j (positions of 0 in A053839, 1-based)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9198 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A053839_a(idx_t n)
{
    seq_t sum = 0;
    idx_t x = n;
    while (x != 0) {
        sum += (x & 3);
        x >>= 2;
    }
    return sum & 3;
}

seq_t a(idx_t n)
{
    idx_t j = n - 1;
    seq_t s = A053839_a(j);
    seq_t d = (4 - s) & 3;
    return (seq_t)n * 4 - 3 + d;
}
