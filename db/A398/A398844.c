/*
 * A398844  Lower (1/2, 1/3)-midsequence of triangular numbers (A000217) and
 *          pentagonal numbers (A000326); see Comments.
 * https://oeis.org/A398844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((9*n^2 + n)/12)
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lower (1/2, 1/3)-midsequence of triangular and pentagonal numbers. */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;           /* convert index to seq_t */
    seq_t sq = nn * nn;            /* n^2 */
    seq_t term = ((seq_t)9 * sq + nn) / ((seq_t)12); /* floor division */
    return term;
}
