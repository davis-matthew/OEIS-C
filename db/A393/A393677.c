/*
 * A393677  Upper (1,1/2) midsequence of (n^2) and ((n+1)^2); see Comments.
 * https://oeis.org/A393677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((3*n^2 + 2*n + 1)/2) = (3*n^2 + 2*n + 2)/2
 * Range:     u8 n <= 8, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast the index to the sequence type */
    seq_t val = nn * nn;          /* n² */
    val = val * (seq_t)3;         /* 3·n² */
    val += 2 * nn;                /* + 2·n */
    val += 2;                     /* + 2 */
    return val / 2;               /* division by 2 (integer truncate) */
}
