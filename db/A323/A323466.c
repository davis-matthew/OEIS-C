/*
 * A323466  Number of terms in row n of A323465.
 * https://oeis.org/A323466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 + A000120(n) + (number of runs of 0's in binary n)
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 103 terms through n = 8187 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t x = (n >> 1) & ~n;   /* positions of "10" patterns: one per zero run */
    idx_t y = n;
    unsigned c = 1;
    while (y) { c += (unsigned)(y & 1); y >>= 1; }
    while (x) { c += (unsigned)(x & 1); x >>= 1; }
    return (seq_t)c;
}
