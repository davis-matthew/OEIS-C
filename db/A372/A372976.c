/*
 * A372976  Number of minimum edge covers in the n-double cone graph.
 * https://oeis.org/A372976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^2 + 4*n if (n mod 2 = 0), otherwise 3*n^2
 * Range:     u8 n <= 9, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1u) == 0u)      /* n is even */
        return 3 * nn * nn + 4 * nn;
    else                     /* n is odd */
        return 3 * nn * nn;
}
