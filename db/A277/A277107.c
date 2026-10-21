/*
 * A277107  a(n) = 16*3^n - 48.
 * https://oeis.org/A277107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*3^n - 48
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t val = pow3(n);     /* compute 3^n */
    val *= (seq_t)16;        /* multiply by 16 */
    val -= (seq_t)48;        /* subtract 48 */
    return val;
}
