/*
 * A398849  Upper (-1/3, 1/2)-midsequence of triangular numbers (A000217) and
 *          pentagonal numbers (A000326); see Comments.
 * https://oeis.org/A398849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling(n*(7*n-5)/12)
 * Range:     u8 n <= 6, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    /* Compute n*(7*n-5) carefully */
    seq_t prod = sn * ((seq_t)7 * sn - (seq_t)5);
    /* Ceiling division by 12 */
    return (prod + (seq_t)11) / (seq_t)12;
}
