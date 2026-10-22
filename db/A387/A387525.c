/*
 * A387525  a(n) = (n^4-3*n^2+4*n+2)/2.
 * https://oeis.org/A387525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 - 3*n^2 + 4*n + 2) / 2
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 303, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n_sq  = nn * nn;          /* n^2 */
    seq_t n4    = n_sq * n_sq;      /* n^4 */
    seq_t numerator = n4 - (seq_t)3 * n_sq + (seq_t)4 * nn + (seq_t)2;
    return numerator / (seq_t)2;
}
