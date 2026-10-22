/*
 * A395025  a(n) = n! * [x^n] exp(x-x^2/2)/(1 - x).
 * https://oeis.org/A395025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*a(n-1) - 2*(n-1)*a(n-2) + (n-1)*(n-2)*a(n-3) * with
 *            initial values a(0)=1, a(1)=2, a(2)=4
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This recurrence follows from the generating function
 * exp(x-x^2/2)/(1-x). */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;

    seq_t a_nm3 = (seq_t)1; /* a(0) */
    seq_t a_nm2 = (seq_t)2; /* a(1) */
    seq_t a_nm1 = (seq_t)4; /* a(2) */

    for (idx_t i = 3; i <= n; ++i)
    {
        /* term1 = (i+1) * a[i-1] */
        seq_t term1 = ((seq_t)(i + 1)) * a_nm1;

        /* term2 = 2 * (i-1) * a[i-2] */
        seq_t term2 = ((seq_t)2 * (i - 1)) * a_nm2;

        /* term3 = (i-1) * (i-2) * a[i-3] */
        seq_t term3 = ((seq_t)(i - 1) * (i - 2)) * a_nm3;

        /* a[i] = term1 - term2 + term3 */
        seq_t a_cur = term1 - term2 + term3;

        /* shift for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_cur;
    }

    return a_nm1; /* a(n) */
}
