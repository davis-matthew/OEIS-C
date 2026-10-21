/*
 * A294259  a(n) = n*(n^3 + 2*n^2 - 5*n + 10)/8.
 * https://oeis.org/A294259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^3 + 2*n^2 - 5*n + 10)/8
 * Range:     u8 n <= 5, u16 n <= 26, u32 n <= 281, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t nn2 = nn * nn;
    seq_t nn3 = nn2 * nn;
    seq_t expr = nn3 + (seq_t)2 * nn2 - (seq_t)5 * nn + (seq_t)10;
    return nn * expr / (seq_t)8;
}
