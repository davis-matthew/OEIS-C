/*
 * A393148  Partial sums of truncated triangular pyramid numbers.
 * https://oeis.org/A393148
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/24)*(n-3)*(n-2)*(n^2 + 11*n - 48)
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t
a(idx_t n)
{
    seq_t nn = (seq_t)n;           /* convert index to seq_t type */
    seq_t term = (nn - (seq_t)3) * (nn - (seq_t)2);   /* (n-3)*(n-2)   */
    term = term * (nn * nn + (seq_t)11 * nn - (seq_t)48); /* * (n^2+11n-48) */
    term = term / (seq_t)24;       /* divide by 24     */
    return term;
}
