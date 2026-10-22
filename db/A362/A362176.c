/*
 * A362176  Expansion of e.g.f. exp(x * (1-2*x)).
 * https://oeis.org/A362176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - 4*(n-1)*a(n-2), a(0)=1, a(1)=1
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 14, i64 n <= 24, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)1;   /* a(1) */
    seq_t a_curr;
    for (idx_t i = 2; i <= n; ++i) {
        /* compute a(i) = a(i-1) - 4*(i-1)*a(i-2) */
        a_curr = a_prev1 - ((seq_t)4 * (seq_t)(i - 1) * a_prev2);
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_prev1;
}
