/*
 * A340849  a(n) = A001045(n) + A052928(n).
 * https://oeis.org/A340849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(2^n/3)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* compute 2^n as a seq_t */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);

    /* remainder depending on parity of n:
       2^n mod 3 = 1 for even n, 2 for odd n                */
    seq_t r = (n & 1) ? (seq_t)2 : (seq_t)1;

    /* floor(2^n/3) = (2^n - r) / 3 */
    seq_t div = (pow2 - r) / 3;

    /* a(n) = n + floor(2^n/3) */
    return (seq_t)n + div;
}
