/*
 * A284574  a(n) = A048724(n) mod 3.
 * https://oeis.org/A284574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A048724(n) mod 3, where A048724(n) = n XOR 2n
 * Range:     u8 n <= 1418, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 137 terms through n = 7902 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A048724_a(idx_t n)
{
    return (seq_t)n ^ ((seq_t)n << 1);
}

seq_t a(idx_t n)
{
    return A048724_a(n) % 3;
}
