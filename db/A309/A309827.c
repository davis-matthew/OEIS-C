/*
 * A309827  a(n) is the square of the number consisting of one 1 and n 6's:
 *          (166...6)^2.
 * https://oeis.org/A309827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ((5*10^n - 2)/3)^2
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the square of the decimal number consisting of one 1 and n 6's. */
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t pow10n = pow10(n);               /* 10^n as a seq_t */
    seq_t v = ((seq_t)5 * pow10n - (seq_t)2) / (seq_t)3;
    return v * v;
}
