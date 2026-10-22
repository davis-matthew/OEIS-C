/*
 * A375253  Expansion of (1 - 2*x + 2*x^2)/(1 - 2*x - 3*x^2)^(7/2).
 * https://oeis.org/A375253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 5 a(n) = ((n+4)/(n*(n+2))) * ((2*n+1)*a(n-1) +
 *            3*(n+3)*a(n-2)) (n ≥ 2) The implementation evaluates this
 *            recurrence iteratively, using only integer arithmetic on the
 *            type seq_t
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)5;   /* a(1) */
    seq_t a_curr   = a_prev1;   /* placeholder */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t ni = (seq_t)i;              /* cast once to seq_t to avoid overflow */
        /* compute (2*i + 1)*a_prev1 + 3*(i + 3)*a_prev2 */
        seq_t term1 = ((seq_t)2 * ni + (seq_t)1) * a_prev1;
        seq_t term2 = ((seq_t)3 * (ni + (seq_t)3)) * a_prev2;
        seq_t sum   = term1 + term2;
        /* compute numerator and denominator */
        seq_t numerator   = (ni + (seq_t)4) * sum;
        seq_t denominator = ni * (ni + (seq_t)2);
        a_curr = numerator / denominator;  /* integer division yields exact result */
        /* shift for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
