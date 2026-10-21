/*
 * A263689  a(n) = (2*n^6 - 6*n^5 + 5*n^4 - n^2 + 12)/12.
 * https://oeis.org/A263689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^6 - 6*n^5 + 5*n^4 - n^2 + 12)/12
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t n5 = n4 * nn;
    seq_t n6 = n5 * nn;
    seq_t num = (seq_t)2 * n6 - (seq_t)6 * n5 + (seq_t)5 * n4 - n2 + (seq_t)12;
    return num / (seq_t)12;
}
