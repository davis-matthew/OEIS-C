/*
 * A267886  Decimal representation of the n-th iteration of the "Rule 235"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4, a(2)=27, a(n)=2^(2*n+1)-1 for n>2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)27;
    idx_t exp = 2*n + 1;
    seq_t v = pow2(exp);
    return v - (seq_t)1;
}
