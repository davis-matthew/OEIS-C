/*
 * A273322  Wiener index of graphs of f.c.c. unit cells in a line = Sum of
 *          distances in face-centered cubic grid unit cells connected in a
 *          row.
 * https://oeis.org/A273322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*n^3 + 45*n^2 + 62*n + 16
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 541, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn      = (seq_t)n;
    seq_t n2      = nn * nn;
    seq_t n3      = n2 * nn;
    seq_t result  = (seq_t)27 * n3 + (seq_t)45 * n2 + (seq_t)62 * nn + (seq_t)16;
    return result;
}
