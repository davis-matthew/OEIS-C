/*
 * A292545  Number of 6-cycles in the n-Sierpinski tetrahedron graph.
 * https://oeis.org/A292545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n = 1, 218 for n = 2, a(n) = 219*4^(n-2) for n > 2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n == 1U) return (seq_t)0;
    if (n == 2U) return (seq_t)218;
    /* n > 2 */
    seq_t power = pow4(n - 2);       /* compute 4^(n-2) */
    return power * (seq_t)219;       /* multiply by 219 */
}
