/*
 * A393441  a(n) = (n^4-n^3+2*n^2-2*n+2)/2.
 * https://oeis.org/A393441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 - n^3 + 2*n^2 - 2*n + 2)/2
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t hn = (seq_t)n;
    seq_t n2 = hn * hn;
    seq_t n3 = n2 * hn;
    seq_t n4 = n2 * n2;
    seq_t term = n4 - n3 + ((seq_t)2 * n2) - ((seq_t)2 * hn) + (seq_t)2;
    return term / (seq_t)2;
}
