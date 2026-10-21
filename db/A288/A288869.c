/*
 * A288869  Numerators of z-sequence for the Sheffer matrix T = P*Lah =
 *          A271703 = A007318*A271703 = (exp(t), t/(1-t)).
 * https://oeis.org/A288869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, frac
 * Formula:   a(0)=1, a(1)=-1; for n>=2 a(n) = -2*n*a(n-1) - n*(n-2)*a(n-2)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 10, i64 n <= 18, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;
    seq_t a0 = (seq_t)1;       /* a(0) */
    seq_t a1 = (seq_t)-1;      /* a(1) */
    for (idx_t k = 2; k <= n; ++k) {
        seq_t term1 = ((seq_t)2 * k) * a1;          /* 2*k*a(k-1) */
        seq_t term2 = ((seq_t)k * (k - 2)) * a0;     /* k*(k-2)*a(k-2) */
        seq_t a2  = -term1 - term2;                 /* a(k) */
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
