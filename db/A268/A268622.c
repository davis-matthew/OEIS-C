/*
 * A268622  Number of n X 2 0..2 arrays with some element plus some
 *          horizontally or vertically adjacent neighbor totalling two no more
 *          than once.
 * https://oeis.org/A268622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=9; for n>1, a(n)=2*3^(n-2)*(5+8*n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)9;          /* base value */
    if (n == 0)
        return (seq_t)0;          /* defensive, out of offset */

    /* compute 3^(n-2) */
    seq_t pow3 = seq_pow((seq_t)3, n - 2);

    /* compute (5 + 8*n) */
    seq_t temp = (seq_t)5 + (seq_t)8 * (seq_t)n;

    /* final result: 2 * pow3 * temp */
    seq_t res = (seq_t)2 * pow3 * temp;
    return res;
}
