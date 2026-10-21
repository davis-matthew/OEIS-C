/*
 * A266302  Decimal representation of the n-th iteration of the "Rule 15"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is even; a(n) = 2*4^n - 2 if n is odd
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Even indices give the constant value 1. */
    if ((n & 1U) == 0U)
        return (seq_t)1;

    /* Odd indices: 2·4^n – 2. */
    seq_t val = pow_uint(4ULL, n);   /* 4^n */
    val = val * (seq_t)2;           /* multiply by 2 */
    val = val - (seq_t)2;           /* subtract 2 */
    return val;
}
