/*
 * A369405  Context-free language 1^n.0^(2n).
 * https://oeis.org/A369405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^n-1)*10^(2*n)/9
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Ensure n >= 1; behaviour for n < 1 is undefined but harmless */
    seq_t pow10_n  = pow10(n);    /* 10^n */
    seq_t pow10_2n = pow10(2*n);  /* 10^(2n) */

    /* (10^n - 1) / 9 is an integer equal to 111...1 (n 1's) */
    seq_t ones = (pow10_n - 1) / 9;

    return ones * pow10_2n;
}
