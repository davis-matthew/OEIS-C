/*
 * A400537  Number of lattice points of the 24-cell whose vertices are the
 *          permutations of (+-n, +-n, 0, 0).
 * https://oeis.org/A400537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = (24*n^4 + 32*n^3 + 24*n^2 + 16*n + 3)/3
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 150, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2   = nn * nn;          /* n^2   */
    seq_t n3   = n2 * nn;          /* n^3   */
    seq_t n4   = n2 * n2;          /* n^4   */
    seq_t term = 24 * n4 + 32 * n3 + 24 * n2 + 16 * nn + 3;
    return term / 3;
}
