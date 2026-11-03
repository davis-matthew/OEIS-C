/*
 * A319603  a(n) = n^3 + reversal of digits of n^3.
 * https://oeis.org/A319603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = n^3 + reverse_decimal(n^3)
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 1001, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 9659 (2026-11-01)
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/reverse_digits.h"

seq_t a(idx_t n) {
    seq_t z = (seq_t)n;
    seq_t cube = z * z * z;
    seq_t rev = reverse_digits(cube);
    return cube + rev;
}
