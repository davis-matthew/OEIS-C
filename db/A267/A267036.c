/*
 * A267036  Decimal representation of the n-th iteration of the "Rule 85"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((-1)^n-1)/2 + 4^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t pow4 = pow_uint(4, n);
    /* (-1)^n is +1 for even n, -1 for odd n;  ((-1)^n-1)/2 is 0 or -1 */
    return (n & 1) ? (pow4 - 1) : pow4;
}
