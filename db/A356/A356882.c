/*
 * A356882  E.g.f. satisfies: A(x) * log(A(x)) = x * (exp(x*A(x)) - 1).
 * https://oeis.org/A356882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} (n-2*k+1)^(k-1) *
 *            Stirling2(n-k,k)/(n-k)!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses dynamic programming for Stirling numbers of the second kind
   and reformulates the summand to avoid fractional division by multiplying
   by the factor (n!/(n-k)!). */

#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* max number of columns needed for Stirling computation */
    idx_t maxk = n / 2;

    /* allocate two rows for Stirling DP */
    seq_t *prev = (seq_t *)calloc(maxk + 1, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(maxk + 1, sizeof(seq_t));
    if (!prev || !curr) return (seq_t)0;  /* allocation failure */

    prev[0] = (seq_t)1;  /* S(0,0) = 1 */

    seq_t sum = 0;

    for (idx_t m = 1; m <= n; ++m) {
        curr[0] = 0;
        idx_t limit = (m < maxk) ? m : maxk;

        /* compute S(m,k) for k = 1..limit */
        for (idx_t k = 1; k <= limit; ++k) {
            curr[k] = seq_mul(prev[k], (seq_t)k) + prev[k - 1];
        }

        /* check if we need to add contribution for k = n - m */
        idx_t k_needed = n - m;           /* k such that m = n - k */
        if (k_needed >= 1 && k_needed <= maxk && k_needed <= limit) {
            /* (n - 2*k + 1)^(k-1) */
            seq_t pow_term = seq_pow((seq_t)(n - 2 * k_needed + 1),
                                     (idx_t)(k_needed - 1));

            /* Stirling value */
            seq_t stirling = curr[k_needed];

            /* term = pow_term * stirling * product_{i=1..k}(n-k+i) */
            seq_t term = seq_mul(pow_term, stirling);
            for (idx_t i = 1; i <= k_needed; ++i) {
                term = seq_mul(term, (seq_t)(n - k_needed + i));
            }

            sum += term;
        }

        /* swap rows for next iteration */
        seq_t *temp = prev;
        prev = curr;
        curr = temp;
    }

    free(prev);
    free(curr);
    return sum;
}
