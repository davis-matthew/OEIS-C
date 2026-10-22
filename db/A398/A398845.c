/*
 * A398845  Upper (1/2, 1/3)-midsequence of triangular numbers (A000217) and
 *          pentagonal numbers (A000326); see Comments.
 * https://oeis.org/A398845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n*(9*n+1)/12)
 * Range:     u8 n <= 5, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn  = (seq_t)n;
    seq_t num = tn * (((seq_t)9) * tn + (seq_t)1);
    return (num + (seq_t)11) / (seq_t)12;
}
