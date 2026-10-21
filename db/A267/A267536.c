/*
 * A267536  Decimal representation of the n-th iteration of the "Rule 143"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*2^(2*n-3)-1 for n > 1, a(0)=1, a(1)=6
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;
    if (n == 1u)
        return (seq_t)6;
    idx_t exp = 2*n - 3;
    seq_t pow = pow2(exp);
    return ((seq_t)13 * pow) - (seq_t)1;
}
