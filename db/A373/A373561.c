/*
 * A373561  a(n) = (1/3)*n^5 + (1/2)*n^4 + (1/6)*n^3.
 * https://oeis.org/A373561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3*(n+1)*(2n+1)/6
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t nn2 = nn * nn;
    seq_t nn3 = nn2 * nn;
    seq_t term = nn3 * (nn + (seq_t)1) * (((seq_t)2 * nn) + (seq_t)1) / (seq_t)6;
    return term;
}
