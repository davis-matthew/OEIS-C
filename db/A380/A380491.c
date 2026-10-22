/*
 * A380491  a(n) = n! * Sum_{k=0..n} binomial(2*n-3,k)/(n-k)!.
 * https://oeis.org/A380491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} binomial(2*n-3,k)/(n-k)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)0;

    /* For n >= 2, compute iteratively:
       a(n) = sum_{k=0}^n C(2n-3,k) * P(n,k)
       where P(n,k) = n!/(n-k)! = n*(n-1)*...*(n-k+1)
       Term recurrence:
       T_0 = 1
       T_{k+1} = T_k * (2n-3 - k) * (n - k) / (k+1)   (for k = 0..n-1)   */
    idx_t two_n_minus_3 = 2 * n - 3;
    seq_t sum   = 1;   /* k = 0 term */
    seq_t term  = 1;

    for (idx_t k = 0; k < n; ++k) {
        idx_t num1 = two_n_minus_3 - k;   /* >= 0 for n >= 2 */
        idx_t num2 = n - k;
        idx_t denom = k + 1;

        /* term_{k+1} = term_k * num1 * num2 / denom */
        term = term * (seq_t)num1 * (seq_t)num2 / (seq_t)denom;
        sum += term;
    }
    return sum;
}
