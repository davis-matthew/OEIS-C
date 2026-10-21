/*
 * A266873  Decimal representation of the n-th iteration of the "Rule 77"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((-1)^n + 3*2^(2*n+1) + (-1)^n*2^(2*n+1) - 3)/6
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Exponent for the power of two: 2*n+1 */
    idx_t exp = 2*n + 1;
    seq_t pow = pow2(exp);          /* pow = 2^(2*n+1) */

    if ((n & 1) == 0)               /* n is even */
    {
        /* For even n: a = (4*pow - 2) / 6 */
        return ((pow * (seq_t)4 - (seq_t)2) / (seq_t)6);
    }
    else                            /* n is odd */
    {
        /* For odd n: a = (2*pow - 4) / 6 */
        return ((pow * (seq_t)2 - (seq_t)4) / (seq_t)6);
    }
}
