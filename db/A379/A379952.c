/*
 * A379952  a(n) = 1 if n is an odd number or a perfect square, otherwise 0.
 * https://oeis.org/A379952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is odd or a perfect square, otherwise 0
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 138 terms through n = 8234 (2026-11-01)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt.h"

seq_t a(idx_t n) {
    if (n & 1ULL)
        return (seq_t)1;
    idx_t r = isqrt(n);
    if (r * r == n)
        return (seq_t)1;
    return (seq_t)0;
}
