/*
 * A397980  Lower (1/3,1) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A397980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * (7*n + 1) / 6)
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = floor(1/3 * T(n) + S(n)) where T(n) is the triangular number and S(n) is n^2. */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (nn * ((seq_t)7 * nn + 1)) / 6;
}
