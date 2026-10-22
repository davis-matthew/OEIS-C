/*
 * A332243  Starhex honeycomb numbers: a(n) = 13 + 60*n + 60*n^2.
 * https://oeis.org/A332243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 13 + 60*n + 60*n^2
 * Range:     u8 n <= 1, u16 n <= 32, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 13 + 60 * nn + 60 * nn * nn;
}
