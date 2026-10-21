/*
 * A268099  a(n) = 2^(n mod 2)*5*10^floor(n/2) - 1.
 * https://oeis.org/A268099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n mod 2)*5*10^floor(n/2) - 1
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* floor(n/2) */
    idx_t e = n / 2;

    /* 10^floor(n/2) */
    seq_t pow10e = pow10(e);

    /* Multiply by 5 if n is even, by 10 if n is odd */
    seq_t mul = (n & 1ULL) ? (pow10e * (seq_t)10) : (pow10e * (seq_t)5);

    /* Subtract 1 to get the final term */
    return mul - (seq_t)1;
}
