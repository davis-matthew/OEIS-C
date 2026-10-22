/*
 * A344504  a(n) = [x^n] ((x - 1)/sqrt(4/(x + 1) - 3) + x + 1)/(2*x*(3*x -
 *          1)).
 * https://oeis.org/A344504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: (n+1)*(2*n-3)*a(n) = (10*n^2-5*n-12)*a(n-1)
 *                        - 3*(2*n+5)*(n-1)*a(n-2)
 *                        - 9*(2*n-1)*(n-2)*a(n-3)
 * Initial values: a(0)=0, a(1)=1, a(2)=6, a(3)=26 */

seq_t a(idx_t n)
{
    /* handle the initial four terms explicitly */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 6;
    if (n == 3) return 26;

    /* values needed for the recurrence window:
     * a_nm1 = a(n-1), a_nm2 = a(n-2), a_nm3 = a(n-3)  (for the current i) */
    seq_t a_nm3 = 1;   /* a(1) */
    seq_t a_nm2 = 6;   /* a(2) */
    seq_t a_nm1 = 26;  /* a(3) */
    seq_t a_n   = 26;  /* placeholder; will be overwritten */

    for (idx_t i = 4; i <= n; ++i) {
        /* Convert index to seq_t for safe arithmetic */
        seq_t ii = (seq_t)i;

        /* compute coefficients as seq_t to avoid overflow in idx_t multiplication */
        seq_t coef1 = (seq_t)10 * ii * ii - (seq_t)5 * ii - (seq_t)12;
        seq_t coef2 = (seq_t)3 * ((seq_t)2 * ii + (seq_t)5) * (ii - (seq_t)1);
        seq_t coef3 = (seq_t)9 * ((seq_t)2 * ii - (seq_t)1) * (ii - (seq_t)2);

        /* compute numerator: coef1*a(n-1) - coef2*a(n-2) - coef3*a(n-3) */
        seq_t term1 = a_nm1 * coef1;
        seq_t term2 = a_nm2 * coef2;
        seq_t term3 = a_nm3 * coef3;
        seq_t num   = term1 - term2 - term3;

        /* denominator: (i+1)*(2*i-3) */
        seq_t denom = (ii + (seq_t)1) * ((seq_t)2 * ii - (seq_t)3);

        a_n = num / denom;

        /* slide the window for the next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
