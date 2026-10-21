/*
 * A242983  a(n) = (n/2) * (n^3 - 2*n^2 - 2*n + 5).
 * https://oeis.org/A242983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^3 - 2*n^2 - 2*n + 5)/2
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 271, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t expr = n3 - (seq_t)2 * n2 - (seq_t)2 * nn + (seq_t)5;
    return (nn * expr) / (seq_t)2;
}
