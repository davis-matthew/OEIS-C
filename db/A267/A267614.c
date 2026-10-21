/*
 * A267614  Decimal representation of the n-th iteration of the "Rule 185"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 1; for n > 1, a(n) = 3*4^(n-1) - 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    idx_t exp = (idx_t)2 * (n - 1);   // exponent for 4^(n-1) = 2^(2(n-1))
    seq_t val = pow2(exp);           // compute 2^(2(n-1)) == 4^(n-1)
    val = val * (seq_t)3;            // multiply by 3
    val = val - (seq_t)1;            // subtract 1
    return val;
}
