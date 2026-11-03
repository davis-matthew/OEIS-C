/*
 * A256755  a(n) = bitwise OR of n and the reverse of n.
 * https://oeis.org/A256755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = n | rev(n)
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 8471 (2026-11-01)
 * Helpers:   reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/reverse_digits_idx.h"

seq_t a(idx_t n)
{
    return (seq_t)n | (seq_t)reverse_digits_idx(n);
}
