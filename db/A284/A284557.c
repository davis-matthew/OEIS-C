/*
 * A284557  a(n) = A048727(n) mod 3.
 * https://oeis.org/A284557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (n XOR 2n XOR 4n) mod 3, i.e. A048727(n) mod 3
 * Range:     u8 n <= 63, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 137 terms through n = 8187 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A048727_a(idx_t n)
{
    seq_t x = (seq_t)n;
    return x ^ (x << 1) ^ (x << 2);
}

seq_t a(idx_t n)
{
    return A048727_a(n) % 3;
}
