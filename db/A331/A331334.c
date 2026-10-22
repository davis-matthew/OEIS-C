/*
 * A331334  a(n) = n! * [x^n] exp(1 - 1/(2*x + 1))/(2*x + 1).
 * https://oeis.org/A331334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 4*(1 - n)*((n - 1)*a(n - 2) + a(n - 1)), with a(0) = 1,
 *            a(1) = 0
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 10, i64 n <= 17, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* a_nm2 = a(n-2), a_nm1 = a(n-1) */
    seq_t a_nm2 = (seq_t)1;   /* a(0)   */
    seq_t a_nm1 = (seq_t)0;   /* a(1)   */

    for (idx_t k = 2; k <= n; ++k) {
        seq_t k_seq = (seq_t)k;
        /* inner = (k-1)*a_nm2 + a_nm1 = (n-1)*a(n-2) + a(n-1) */
        seq_t inner = ((k_seq - (seq_t)1) * a_nm2) + a_nm1;
        /* a_n = 4*(1 - k)*inner */
        seq_t a_n = (seq_t)4 * ((seq_t)1 - k_seq) * inner;

        /* shift values for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
