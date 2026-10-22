/*
 * A390343  Upper (3/2)-midsequence of binomial(n,3) and binomial(n,2); see
 *          Comments.
 * https://oeis.org/A390343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((n^3 - n)/4) which is equivalent to (n^3 - n + 3)/4
 *            for integer n ≥ 0
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                    /* cast the index to seq_t */
    seq_t n2 = nn * nn;                    /* n^2 */
    seq_t n3 = n2 * nn;                    /* n^3 */
    seq_t numerator = n3 - nn + (seq_t)3;   /* n^3 - n + 3 */
    seq_t result   = numerator / ((seq_t)4);/* integer division gives ceil((n^3-n)/4) */
    return result;
}
