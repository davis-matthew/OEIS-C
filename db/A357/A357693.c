/*
 * A357693  Expansion of e.g.f. cos( sqrt(2) * log(1+x) ).
 * https://oeis.org/A357693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -(2*n-3)*a(n-1) - (n^2-4*n+6)*a(n-2), a(0) = 1, a(1) = 0
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 13, i64 n <= 20, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)0;

    seq_t a0 = (seq_t)1;   /* a(n-2)  */
    seq_t a1 = (seq_t)0;   /* a(n-1)  */

    for (idx_t k = 2; k <= n; ++k)
    {
        seq_t coef1 = (seq_t)(2 * k) - (seq_t)3;                 /* 2*k-3  */
        seq_t coef2 = (seq_t)k * (seq_t)k - (seq_t)4 * (seq_t)k + (seq_t)6;  /* k^2-4k+6 */

        seq_t cur = -(coef1 * a1) - (coef2 * a0);   /* recurrence */

        a0 = a1;
        a1 = cur;
    }
    return a1;
}
