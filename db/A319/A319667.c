/*
 * A319667  Palindromes a(n) = (10^n + 1)*(10^(n+1) + 1).
 * https://oeis.org/A319667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (10^n + 1)*(10^(n+1) + 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute 10^n and 10^(2n+1) */
    seq_t pow_n     = pow10(n);
    seq_t pow_2n1   = pow10(2*n + 1);

    /* a(n) = 10^(2n+1) + 11*10^n + 1 */
    return pow_2n1 + pow_n * 11 + 1;
}
