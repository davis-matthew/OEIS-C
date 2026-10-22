/*
 * A383700  Coefficient of x^2 in expansion of (x+1) * (x+5) * ... *
 *          (x+4*n-3).
 * https://oeis.org/A383700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^2 in (x+1)(x+5)...(x+4n-3). Recurrence:
 *            a(n) = (12n-21) a(n-1) - (48n^2-216n+247) a(n-2) + (4n-11)^3
 *            a(n-3); Initial values: a(0)=0, a(1)=0, a(2)=1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0 || n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* a(n-3), a(n-2), a(n-1) */
    seq_t a_nm3 = (seq_t)0;   /* a(0) */
    seq_t a_nm2 = (seq_t)0;   /* a(1) */
    seq_t a_nm1 = (seq_t)1;   /* a(2) */
    seq_t a_i   = 0;

    for (idx_t i = 3; i <= n; ++i) {
        /* Compute coefficients using idx_t arithmetic first */
        idx_t coef1 = 12 * i - 21;                       /* 12i-21 */
        idx_t coef2 = 48 * i * i - 216 * i + 247;        /* 48i^2-216i+247 */
        idx_t m     = 4 * i - 11;                         /* 4i-11 */

        /* Convert to seq_t for the multiplication */
        seq_t term1 = ((seq_t)coef1) * a_nm1;
        seq_t term2 = ((seq_t)coef2) * a_nm2;
        seq_t m3    = ((seq_t)m) * ((seq_t)m) * ((seq_t)m);   /* (4i-11)^3 */
        seq_t term3 = m3 * a_nm3;

        /* a_i = term1 - term2 + term3  (term1+term3 >= term2 for n>=3) */
        a_i = term1;
        a_i += term3;
        a_i -= term2;

        /* Slide the window for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_i;
    }
    return a_nm1;
}
