/*
 * A274766  Multiplication of pair of contiguous repunits, i.e., (0*1, 1*11,
 *          11*111, 111*1111, 1111*11111, ...).
 * https://oeis.org/A274766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^n - 1)*(10^(n+1) - 1)/81
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t ten_pow_n   = pow10(n);
    seq_t ten_pow_n1  = pow10(n + 1);
    seq_t rep_n   = (ten_pow_n - 1) / 9;
    seq_t rep_n1  = (ten_pow_n1 - 1) / 9;
    return rep_n * rep_n1;
}
