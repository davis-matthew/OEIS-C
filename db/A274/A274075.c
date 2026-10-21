/*
 * A274075  Sum of n-th powers of the roots of x^3 + x^2 - 9*x - 1.
 * https://oeis.org/A274075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + 9*a(n-2) + a(n-3), a(0) = 3, a(1) = -1, a(2) =
 *            19
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 17, i64 n <= 34, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)19;

    seq_t a_nm3 = (seq_t)3;   /* a(n-3)   */
    seq_t a_nm2 = (seq_t)-1;  /* a(n-2)   */
    seq_t a_nm1 = (seq_t)19;  /* a(n-1)   */

    seq_t a_n;
    for (idx_t i = 3; i <= n; ++i) {
        a_n = -a_nm1 + (seq_t)9 * a_nm2 + a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
