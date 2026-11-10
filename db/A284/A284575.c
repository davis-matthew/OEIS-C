/*
 * A284575  a(n) = A048725(n) mod 3.
 * https://oeis.org/A284575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A048725(n) mod 3, where A048725(n) = n XOR 4n
 *            (carryless); computed bit by bit using 2^k == (-1)^k (mod 3)
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 137 terms through n = 7911 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    long long s = 0;
    for (int k = 0; k < 70; k++) {
        int b1 = (k < 64) ? (int)((n >> k) & 1) : 0;
        int b2 = (k >= 2 && k - 2 < 64) ? (int)((n >> (k - 2)) & 1) : 0;
        int r = b1 ^ b2;
        if (r) s += (k & 1) ? -1 : 1;
    }
    s %= 3;
    if (s < 0) s += 3;
    return (seq_t)s;
}
