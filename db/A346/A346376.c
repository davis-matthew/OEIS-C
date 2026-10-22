/*
 * A346376  a(n) = n^4 + 14*n^3 + 63*n^2 + 98*n + 28.
 * https://oeis.org/A346376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 + 14*n^3 + 63*n^2 + 98*n + 28
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 252, u64 n <= 10000,
 *            u128 n <= 10000, big any n
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
    return n4 + (seq_t)14 * n3 + (seq_t)63 * n2 + (seq_t)98 * nn + (seq_t)28;
}
