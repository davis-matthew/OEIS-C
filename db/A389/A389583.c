/*
 * A389583  Upper (1/2,1/3) midsequence of (n^2) and (n^3); see Comments.
 * https://oeis.org/A389583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n^2/2 + n^3/3) Equivalent integer form: a(n) =
 *            (3*n^2 + 2*n^3 + 5)/6 for n >= 0
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;      /* convert index to sequence type */
    seq_t n2 = x * x;        /* n^2 */
    seq_t n3 = n2 * x;       /* n^3 */
    seq_t numerator = (seq_t)3 * n2 + (seq_t)2 * n3 + (seq_t)5;
    return numerator / 6;    /* integer division yields ceil */
}
