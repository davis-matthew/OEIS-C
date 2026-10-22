/*
 * A393347  Upper (1,1/3) midsequence of (n^2) and (n^3); see Comments.
 * https://oeis.org/A393347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n^2 + n^3/3) = (3*n^2 + n^3 + 2)/3
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t numer = 3 * n2 + n3;   /* 3*n^2 + n^3 */
    return (numer + 2) / 3;      /* ceil division by 3 */
}
