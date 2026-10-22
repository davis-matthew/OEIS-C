/*
 * A307814  Number of solutions mod 3^n to the simultaneous equations 2*(Y^4 +
 *          T^4) + X^4 + Z^4 = 2*(X^2 + Z^2)*(Y^2 + T^2) (mod 3^n), U^2 + Y^2
 *          = X^2 + Z^2 (mod 3^n), T = 0 (mod 3), X^2 = Y^2 = Z^2 = U^2 = 1
 *          (mod 3).
 * https://oeis.org/A307814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Formula: a(1)=16, a(2)=16*3^4, a(3)=16*3^7,
   a(n)=16*11*3^(3n-4) for n>=4 */
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)16 * seq_pow((seq_t)3, 4);
    if (n == 3) return (seq_t)16 * seq_pow((seq_t)3, 7);
    /* n >= 4 */
    idx_t exp = 3 * n - 4;
    seq_t p = seq_pow((seq_t)3, exp);
    return (seq_t)16 * (seq_t)11 * p;
}
