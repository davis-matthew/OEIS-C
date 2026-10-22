/*
 * A389582  Lower (1/2,1/3) midsequence of (n^2) and (n^3); see Comments.
 * https://oeis.org/A389582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((1/2)*n^2 + (1/3)*n^3) = (2*n^3 + 3*n^2) / 6
 *            (integer division)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns  = (seq_t)n;           /* n as seq_t */
    seq_t n2  = ns * ns;            /* n^2 */
    seq_t n3  = n2 * ns;            /* n^3 */
    seq_t num = (seq_t)2 * n3 +      /* 2*n^3 */
                (seq_t)3 * n2;       /* + 3*n^2 */
    return num / (seq_t)6;           /* floor division by 6 */
}
