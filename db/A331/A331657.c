/*
 * A331657  a(n) = Sum_{k=0..n} (-1)^(n - k) * binomial(n,k) * binomial(n+k,k)
 *          * n^k.
 * https://oeis.org/A331657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n - k) * binomial(n,k) *
 *            binomial(n+k,k) * n^k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use iterative computation of binomials and powers:
       cur_nk   = binomial(n,   k)
       cur_nk_k = binomial(n+k, k)
       cur_pow  = n^k
       Sum positives and negatives separately to avoid
       unsigned underflow. */
    idx_t i;
    seq_t cur_nk   = (seq_t)1;   /* binomial(n,0)   */
    seq_t cur_nk_k = (seq_t)1;   /* binomial(n+0,0) */
    seq_t cur_pow  = (seq_t)1;   /* n^0            */
    seq_t sum_pos  = (seq_t)0;
    seq_t sum_neg  = (seq_t)0;
    seq_t sn = (seq_t)n; /* n as seq_t for multiplication */

    for (i = 0; i <= n; ++i) {
        /* term = cur_nk * cur_nk_k * cur_pow */
        seq_t term = cur_nk * cur_nk_k;
        term = term * cur_pow;

        /* Add to positive or negative accumulators */
        if ((n - i) & 1u) {
            sum_neg += term;
        } else {
            sum_pos += term;
        }

        /* Update for next k (skip after last iteration) */
        if (i == n) break;

        /* cur_nk   = cur_nk   * (n - i) / (i + 1) */
        cur_nk = cur_nk * (seq_t)(n - i) / (seq_t)(i + 1);
        /* cur_nk_k = cur_nk_k * (n + i + 1) / (i + 1) */
        cur_nk_k = cur_nk_k * (seq_t)(n + i + 1) / (seq_t)(i + 1);
        /* cur_pow  = cur_pow  * n */
        cur_pow = cur_pow * sn;
    }

    return sum_pos - sum_neg;
}
