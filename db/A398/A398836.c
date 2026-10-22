/*
 * A398836  Lower (1, 2/3)-midsequence of triangular numbers (A000217) and
 *          pentagonal numbers (A000326); see Comments.
 * https://oeis.org/A398836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n*(9*n+1)/6 )
 * Range:     u8 n <= 5, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to seq_t once */
    seq_t nn = (seq_t)n;
    /* compute n*(9*n+1) as a seq_t product */
    seq_t prod = nn * ((seq_t)9 * nn + (seq_t)1);
    /* integer division truncates towards zero, which is floor for n ≥ 0 */
    return prod / (seq_t)6;
}
