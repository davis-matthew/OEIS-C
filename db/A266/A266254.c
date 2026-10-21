/*
 * A266254  Binary representation of the n-th iteration of the "Rule 11"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(n)=11 for even n>0; a(n)=(10*100^n-100)/9 for odd n>0
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if ((n & 1) == 0)               /* n is even */
        return (seq_t)11;
    /* n is odd >0: compute (10^(2*n+1) - 100) / 9 */
    seq_t num = pow10((idx_t)(2 * n + 1));
    num -= (seq_t)100;
    return num / (seq_t)9;
}
