/*
 * A295382  Expansion of e.g.f. exp(-2*x/(1 - x))/(1 - x).
 * https://oeis.org/A295382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n!*Laguerre(n,2) = n!*Sum_{k=0..n}
 *            (-1)^k*binom(n,k)*2^k/k! Recurrence: a(0)=1; a(1)=-1; for n>=1:
 *            a(n+1) = (2*n-1)*a(n) - n*n*a(n-1)
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 12, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;
    seq_t prev2 = (seq_t)1;          /* a(0) */
    seq_t prev1 = (seq_t)-1;         /* a(1) */
    for (idx_t i = 1; i < n; ++i) { /* compute a(i+1) */
        seq_t coeff1 = (seq_t)(2 * i - 1);
        seq_t coeff2 = (seq_t)i * (seq_t)i;   /* i^2 */
        seq_t cur = coeff1 * prev1 - coeff2 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
