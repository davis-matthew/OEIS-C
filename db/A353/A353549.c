/*
 * A353549  Expansion of e.g.f. log(1+3*x) * exp(x)/3.
 * https://oeis.org/A353549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 15, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 0
 *          a(1) = 1
 *          a(n) = (-3*n + 4) * a(n-1) + 3*(n-1) * a(n-2) + 1
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t a_prev2 = 0;          /* a(n-2)   */
    seq_t a_prev1 = 1;          /* a(n-1)   */
    seq_t a_curr  = 0;          /* a(n)     */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t coeff1 = -3 * (seq_t)i + 4;          /* (-3*i + 4) */
        seq_t coeff2 = 3 * (seq_t)(i - 1);        /* 3*(i-1)   */

        a_curr = coeff1 * a_prev1 + coeff2 * a_prev2 + 1;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
