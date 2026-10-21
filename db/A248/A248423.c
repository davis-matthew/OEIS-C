/*
 * A248423  Multiples of 4 with digits backwards.
 * https://oeis.org/A248423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = reverse_decimal(4*n)
 * Range:     u8 n <= 25, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * 4;
    return reverse_decimal(m);
}
