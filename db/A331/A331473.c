/*
 * A331473  Alternating sum of (n+1)*A000108(n+1).
 * https://oeis.org/A331473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) binomial(2*k+2, k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 12, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simplified recurrence: a(n) = binom(2*n+2, n) - a(n-1) with a(0)=1 */
/* where binom(2*k+2, k) can be computed iteratively as:
 *   C_0 = 1
 *   C_k = C_{k-1} * (2*k+2)*(2*k+1) / (k*(k+2))  for k>0
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* C_k: binomial(2*k+2, k) */
    seq_t C = (seq_t)1;
    /* a_prev holds a(k-1); initialized to a(-1)=0 for convenience */
    seq_t a_prev = (seq_t)0;

    for (idx_t k = (idx_t)0; k <= n; ++k) {
        if (k == (idx_t)0) {
            /* a(0) = C_0 = 1 */
            a_prev = (seq_t)1;
            continue;
        }

        /* Compute next binomial coefficient C_k */
        idx_t kk = k;                           /* k as idx_t */
        seq_t num1 = (seq_t)( (idx_t)2 * kk + (idx_t)2 );
        seq_t num2 = (seq_t)( (idx_t)2 * kk + (idx_t)1 );
        seq_t denom1 = (seq_t)kk;
        seq_t denom2 = (seq_t)(kk + (idx_t)2);

        /* Use the multiplicative recurrence:
         * C_k = C_{k-1} * num1 * num2 / (denom1 * denom2)
         */
        C = (C * num1 * num2) / (denom1 * denom2);

        /* a(k) = C_k - a(k-1) */
        seq_t a = C - a_prev;
        a_prev = a;
    }

    return a_prev;
}
