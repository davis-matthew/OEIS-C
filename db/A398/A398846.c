/*
 * A398846  Lower (1/3, 1/2)-midsequence of triangular numbers (A000217) and
 *          pentagonal numbers (A000326); see Comments.
 * https://oeis.org/A398846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(11*n-1)/12)
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n*(11*n-1) using seq_t arithmetic to avoid overflow. */
    seq_t temp = (seq_t)11 * n;   /* 11*n */
    temp = temp - (seq_t)1;       /* 11*n - 1 */
    seq_t prod = (seq_t)n * temp; /* n*(11*n-1) */
    seq_t result = prod / (seq_t)12; /* floor division, all values non‑negative */
    return result;
}
