/*
 * A388848  Number of distinct squares (of any orientation) whose vertices are
 *          points of the cross of side n.
 * https://oeis.org/A388848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n^4 - 3*n^2 + (n mod 2))/4
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 230, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;
    seq_t n2 = nn * nn;
    seq_t n4 = n2 * n2;
    seq_t res = (6 * n4 - 3 * n2 + (nn & 1)) / 4;
    return res;
}
