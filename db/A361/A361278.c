/*
 * A361278  Expansion of e.g.f. exp(x * (1+x)^2).
 * https://oeis.org/A361278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * SUM_{k=0..n} binomial(2*k, n-k) / k!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 4*(n-1)*a(n-2) + 3*(n-1)*(n-2)*a(n-3)                               */
/* Initial values: a(0)=1, a(1)=1, a(2)=5 */

seq_t a(idx_t n) {
    /* Handle the first three values explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;

    /* keep the last three terms: a(n-3), a(n-2), a(n-1) */
    seq_t a_nm3 = (seq_t)1;   /* a(0) */
    seq_t a_nm2 = (seq_t)1;   /* a(1) */
    seq_t a_nm1 = (seq_t)5;   /* a(2) */
    seq_t cur   = a_nm1;      /* placeholder for a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        /* coefficients depending on current index i */
        seq_t coef2 = (seq_t)4 * (seq_t)(i - 1);                     /* 4*(i-1) */
        seq_t coef3 = (seq_t)3 * (seq_t)(i - 1) * (seq_t)(i - 2);   /* 3*(i-1)*(i-2) */

        /* recurrence: a(n) = a(n-1) + coef2*a(n-2) + coef3*a(n-3) */
        cur = a_nm1 + coef2 * a_nm2 + coef3 * a_nm3;

        /* Shift the window for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return cur;
}
