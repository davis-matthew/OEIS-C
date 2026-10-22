/*
 * A395736  Expansion of (Sum_{k>=0} binomial(5*k,k) * x^k)^5.
 * https://oeis.org/A395736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} 4^k * binomial(k+3,3) * binomial(5*n+4, n-k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses recurrence relations to compute the sum in O(n) time. */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* binomial(5*n+4, n)  is the first binomial factor of the sum */
    seq_t bin_coeff = binomial(5 * n + 4, n);

    seq_t sum   = 0;         /* accumulator for the sum */
    seq_t pow4  = 1;         /* 4^k          */
    seq_t u     = 1;         /* binomial(k+3,3)  */
    seq_t t     = bin_coeff; /* binomial(5n+4, n-k) */

    for (idx_t k = 0; k <= n; ++k) {
        /* add the k-th term */
        sum += pow4 * u * t;

        if (k == n) break; /* finished the last term */

        /* update variables for the next k+1 */
        idx_t nk = n - k;
        /* t_{k+1} = t_k * (n-k) / (4*n+5+k) */
        t = (t * (seq_t)nk) / (seq_t)(4 * n + 5 + k);

        /* u_{k+1} = u_k * (k+4) / (k+1) */
        u = (u * (seq_t)(k + 4)) / (seq_t)(k + 1);

        /* pow4_{k+1} = 4 * pow4_k */
        pow4 = pow4 * (seq_t)4;
    }

    return sum;
}
