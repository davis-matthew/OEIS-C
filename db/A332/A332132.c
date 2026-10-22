/*
 * A332132  a(n) = (10^(2n+1)-1)/3 - 10^n.
 * https://oeis.org/A332132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (10^(2n+1)-1)/3 - 10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute 10^n and 10^(2n+1) */
    seq_t ten_n    = pow10(n);
    seq_t ten_2n1  = pow10(2 * n + 1);

    /* (10^(2n+1)-1)/3 is always an integer */
    seq_t res = (ten_2n1 - 1) / 3;

    /* Subtract 10^n to obtain a(n) */
    res -= ten_n;
    return res;
}
