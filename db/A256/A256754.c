/*
 * A256754  a(n) = bitwise AND of n and the reverse of n.
 * https://oeis.org/A256754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = n AND rev(n)
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 8941 (2026-11-01)
 * Helpers:   reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/reverse_digits_idx.h"

seq_t a(idx_t n)
{
    return (seq_t)n & (seq_t)reverse_digits_idx(n);
}
