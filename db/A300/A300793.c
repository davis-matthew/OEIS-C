/*
 * A300793  a(n) is the n-th derivative of arcsinh(1/x) at x=1 times
 *          (-2)^n/sqrt(2) for n >= 1.
 * https://oeis.org/A300793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4(n-2)^2(n-3)a(n-3) - 2(3n-5)(n-2)a(n-2) + (4n-5)a(n-1)
 *            Initial values: a(1)=1, a(2)=3, a(3)=13
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined, but harmless */

    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)3;
    if (n == 3)
        return (seq_t)13;

    /* a_{n-3}, a_{n-2} and a_{n-1} for the current step */
    seq_t a_nm3 = (seq_t)1;    /* a1 */
    seq_t a_nm2 = (seq_t)3;    /* a2 */
    seq_t a_nm1 = (seq_t)13;   /* a3 */

    for (idx_t i = 4; i <= n; ++i) {
        /* compute the polynomial coefficients */
        seq_t coeff1 = (seq_t)4;
        coeff1 = coeff1 * (seq_t)(i - 2);
        coeff1 = coeff1 * (seq_t)(i - 2);
        coeff1 = coeff1 * (seq_t)(i - 3);

        seq_t coeff2 = (seq_t)2;
        coeff2 = coeff2 * (seq_t)(3 * i - 5);
        coeff2 = coeff2 * (seq_t)(i - 2);

        seq_t coeff3 = (seq_t)4 * (seq_t)i - (seq_t)5;

        /* recurrence: a_n = coeff1*a_{n-3} + coeff3*a_{n-1} - coeff2*a_{n-2} */
        seq_t term1 = coeff1 * a_nm3;
        seq_t term3 = coeff3 * a_nm1;
        seq_t term2 = coeff2 * a_nm2;

        seq_t a_next = term1 + term3 - term2;

        /* shift values for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_next;
    }

    return a_nm1;
}
