/*
 * A299965  Number of triangles in a Star of David of size n.
 * https://oeis.org/A299965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = n*(10*n^2+9*n+1)
 * Range:     u8 n <= 2, u16 n <= 18, u32 n <= 754, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t term = (seq_t)10 * tn * tn + (seq_t)9 * tn + (seq_t)1;
    return tn * term;
}
