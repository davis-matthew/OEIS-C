/*
 * A376478  a(1) = 1, a(2) = 2, and a(n) = 3^(n-2) for n > 2.
 * https://oeis.org/A376478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2, a(n)=3^(n-2) for n>2
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    /* compute 3^(n-2) using the provided big integer power helper */
    return big_pow((seq_t)3, (seq_t)(n - 2));
}
