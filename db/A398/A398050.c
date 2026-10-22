/*
 * A398050  Lower (1,1/5) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A398050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n+1)/2 + n*n/5); Recursive form (not used in
 *            this implementation): a(n) = 2*a(n-1) - a(n-2) + a(n-5) -
 *            2*a(n-6) + a(n-7), with initial values (a(0), …, a(6)) = (0, 1,
 *            3, 7, 13, 20, 28). This implementation uses the direct
 *            closed‑form expression involving triangular numbers and a f...
 * Range:     u8 n <= 18, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t for arithmetic.  The operands are all
       non‑negative, so integer truncation matches floor(). */
    seq_t sn = (seq_t)n;
    seq_t tri = sn * (sn + 1) / 2;       /* triangular number T(n)   */
    seq_t sq_div5 = (sn * sn) / 5;       /* floor(n²/5)              */
    return tri + sq_div5;                /* a(n) = T(n) + floor(n²/5) */
}
