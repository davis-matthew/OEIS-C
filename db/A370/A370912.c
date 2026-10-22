/*
 * A370912  a(n) = n*(n + 2)*(n + 4).
 * https://oeis.org/A370912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = n*(n+2)*(n+4)
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    return N * (N + (seq_t)2) * (N + (seq_t)4);
}
