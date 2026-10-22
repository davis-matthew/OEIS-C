/*
 * A332186  a(n) = 8*(10^(2n+1)-1)/9 - 2*10^n.
 * https://oeis.org/A332186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 8*(10^(2n+1)-1)/9 - 2*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* compute 10^(2n+1) and 10^n */
    seq_t pow2n1 = pow10((idx_t)(2*n + 1));
    seq_t pown   = pow10(n);

    /* (10^(2n+1)-1)/9 is an integer; multiply by 8 */
    seq_t term = (pow2n1 - (seq_t)1) / (seq_t)9;
    term = term * (seq_t)8;

    /* subtract 2*10^n */
    term = term - (seq_t)2 * pown;

    return term;
}
