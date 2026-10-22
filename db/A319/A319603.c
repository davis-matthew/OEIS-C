/*
 * A319603  a(n) = n^3 + reversal of digits of n^3.
 * https://oeis.org/A319603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = n^3 + reverse_decimal(n^3)
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 1001, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_decimal.h"

seq_t a(idx_t n) {
    seq_t z = (seq_t)n;
    seq_t cube = z * z * z;
    seq_t rev = reverse_decimal(cube);
    return cube + rev;
}
