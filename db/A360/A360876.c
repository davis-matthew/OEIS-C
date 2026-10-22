/*
 * A360876  Number of connected dominating sets in the 2 X n rook graph.
 * https://oeis.org/A360876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - 3^n + 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For safety, handle n = 0. */
    if (n == 0)
        return (seq_t)0;
    seq_t term4 = pow4(n);
    seq_t term3 = pow3(n);
    seq_t result = term4 - term3 + (seq_t)2;
    return result;
}
