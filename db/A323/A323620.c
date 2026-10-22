/*
 * A323620  Expansion of e.g.f. 2*sqrt(1 + x)*sinh(sqrt(5)*log(1 +
 *          x)/2)/sqrt(5).
 * https://oeis.org/A323620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -2*(n-2)*a(n-1) - (n^2 - 5*n + 5)*a(n-2) with a(0) = 0,
 *            a(1) = 1
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a_nm2 = (seq_t)0;  /* a(0) */
    seq_t a_nm1 = (seq_t)1;  /* a(1) */
    seq_t a_n   = a_nm1;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t i_seq = (seq_t)i;
        /* term1 = -2*(i-2)*a_nm1 */
        seq_t term1 = -((seq_t)2 * (i_seq - 2) * a_nm1);
        /* term2 = -(i^2 - 5*i + 5)*a_nm2 */
        seq_t poly  = i_seq * i_seq - (seq_t)5 * i_seq + 5;
        seq_t term2 = -(poly * a_nm2);
        /* recurrence */
        a_n   = term1 + term2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
