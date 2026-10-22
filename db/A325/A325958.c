/*
 * A325958  Sum of the corners of a 2n+1 X 2n+1 square spiral.
 * https://oeis.org/A325958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n^2 + 4*n + 4
 * Range:     u8 n <= 3, u16 n <= 63, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute 16*n^2 + 4*n + 4 using seq_t arithmetic */
    seq_t res = (seq_t)16 * nn * nn + (seq_t)4 * nn + (seq_t)4;
    return res;
}
