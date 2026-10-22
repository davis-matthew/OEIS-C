/*
 * A398054  Lower (1,3/5) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A398054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n+1)/2 + (3*n*n)/5)
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lower (1,3/5) midsequence of triangular numbers and squares */
seq_t a(idx_t n)
{
    seq_t s = (seq_t)n * ((seq_t)n + 1) / 2;               /* triangular number T(n) */
    seq_t t = ((seq_t)3 * (seq_t)n * (seq_t)n) / 5;          /* floor(3*n^2/5) */
    return s + t;
}
