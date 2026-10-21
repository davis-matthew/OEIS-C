/*
 * A257042  a(n) = (3*n+7)*n^2.
 * https://oeis.org/A257042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+7)*n^2
 * Range:     u8 n <= 3, u16 n <= 27, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t  = (seq_t)3 * nn + (seq_t)7;
    return t * nn * nn;
}
